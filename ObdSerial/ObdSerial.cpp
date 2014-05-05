#include <fcntl.h>
#include <termios.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include <stdio.h>
#include <cstring>
#include <cstdlib>
#include <sstream>
#include "ObdSerial.h"
#include "obdcomms.h" // pid table
#include "obdconvs.h" // conversion functions
#include "../Observable.h"
#include <algorithm> // sorting
#include <thread> // for multithreading
#include <chrono> // for sleeping

/// add support for reading DTCs

ObdSerial::ObdSerial(const std::string& portpath) : AT_SLEEPTIME(20), NORMAL_OBD_SLEEPTIME(100), EXTRA_LONG_SLEEPTIME(300) {
    //open serial port connection
    char buf[4096];
    // open to read and write, not as the controlling terminal, and in nonblocking mode
    fd = open(portpath.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1) {
        std::cerr << "Failed to open obd port..." << std::endl;
        throw "Error opening OBD2 port";
    }
    // configure the opened serial port
    fcntl(fd, F_SETFL, 0);
    tcgetattr(fd, &options);
    options.c_cflag |= (CLOCAL | CREAD);
    options.c_lflag &= !(ICANON | ECHOE | ISIG);
    options.c_oflag &= !(OPOST);
    cfsetispeed(&options, B38400);
    cfsetospeed(&options, B38400);
    if (tcsetattr(fd, TCSANOW, &options) != 0) {
        std::cerr << "Error setting serial port attributes" << std::endl;
    }
    // configure the ELM327 device
    write(fd, "AT E0\r", sizeof("AT E0\r")); // turn off echo
    std::this_thread::sleep_for(std::chrono::milliseconds(AT_SLEEPTIME));
    read(fd, buf, sizeof(buf));
    write(fd, "AT S0\r", sizeof("AT S0\r")); //turn off spaces
    std::this_thread::sleep_for(std::chrono::milliseconds(AT_SLEEPTIME));
    read(fd, buf, sizeof(buf));
    write(fd, "AT ST 19\r", sizeof("AT ST 19\r")); //lower timeout settings
    std::this_thread::sleep_for(std::chrono::milliseconds(AT_SLEEPTIME));
    read(fd, buf, sizeof(buf));
    write(fd, "AT SH 7E0\r", sizeof("AT SH 7E0\r")); // specify 7E8 device only (engine ECU)
    std::this_thread::sleep_for(std::chrono::milliseconds(AT_SLEEPTIME));
    read(fd, buf, sizeof(buf));

    if (fillSuppdCmds() != 0) {
        std::cerr << "Error getting supported PIDs" << std::endl;
        throw "Error getting supported PIDs";
    }

    VIN = getVINFromCar();

    boolrun = false;
}
ObdSerial::~ObdSerial() {
    //reset device, stop data query thread, close serial port
    boolrun = false;
    std::this_thread::sleep_for(std::chrono::milliseconds(EXTRA_LONG_SLEEPTIME)); // make sure run has time to terminate
    char buf[4096];
    write(fd, "AT Z\r\0", sizeof("AT Z\r\0"));
    std::this_thread::sleep_for(std::chrono::milliseconds(AT_SLEEPTIME));
    read(fd, buf, sizeof(buf));
    close(fd);
}


/// Public interface functions
void ObdSerial::start() {
    if (boolrun == false) {
        // start the data harvesting thread ( start() ) and wait for it to finish
        boolrun = true;
        obdthread = new std::thread(&ObdSerial::run, this);
    }
}
std::vector<int> ObdSerial::getSuppdCmds() {
    return suppdCmds;
}

const std::string& ObdSerial::getVIN() {
    return VIN;
}
void ObdSerial::setFocusedPIDs(const std::vector<int>& fPIDs) {
    for (unsigned int x = 0; x < fPIDs.size(); x++) {
        if ( binary_search(suppdCmds.begin(), suppdCmds.end(), fPIDs.at(x)) == false ) {
            std::cerr << "setFocusedPIDs error: PID " << obdcmds_mode1[x].cmdid << " not supported, skipping" << std::endl;
        }
        else {
            focusPIDs.push_back(fPIDs.at(x));
        }
    }
}

void ObdSerial::stopUpdates() {
    if (boolrun) {
        boolrun = false;
        obdthread->join();
    }
}


/// Internal data query/interpretation methods
// return 0 for success -1 for failure
int ObdSerial::fillSuppdCmds() {
    std::string instr;
    for (int x = 0; x < 4; x++) {
        if (writeToOBD(x*32) != 0) {
            return -1;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(NORMAL_OBD_SLEEPTIME));

        if (readFromOBD(instr) != 0) {
            return -1;
        }
        unsigned long inStrAsLong = strtoul(instr.c_str(), NULL, 16);
        for (int bit = 32; bit > 0; bit--) { // this checks 0-31, then 32-63, etc
            // this is checking if the bit is supported, if it's PID has a conversion function, and if it's within the range of obdcmds_mode1
            if ( ( (inStrAsLong & (1<<bit)) != 0 ) && ( obdcmds_mode1[32-bit + x*32].conv != NULL ) && ((32-bit + x*32) < 100) ) {
                suppdCmds.push_back(32-bit + x*32);
            }
        }
        if (inStrAsLong%2==0) {
            // if it's even, then the next multiple-of-32-PID isn't supported
            // so neither are all the PIDs it could indicate support for, so:
            break;
        }
    }
    std::sort (suppdCmds.begin(), suppdCmds.end());
    return 0;
}
std::string ObdSerial::getVINFromCar() {
    //from wikipedia: 17-char VIN, ASCII-encoded and left-padded with null chars
    char vin[256];
    std::string input, formatted, output;
    write(fd, "09 02\r", sizeof("09 02\r"));
    std::this_thread::sleep_for(std::chrono::milliseconds(EXTRA_LONG_SLEEPTIME));
    read(fd, vin, sizeof(vin));
    input = vin;
    std::istringstream iss(input);
    getline(iss, input);     // skip first line
    while (getline(iss, input) && input.size() > 2) { // concatenate subsequent lines until empty line or ">"
        // this chops off "#:" at beginning of each line - input is now only 1 line long
        // entire original input string is now stored only in iss
        formatted += input.substr(2, input.size());
    }
    formatted = formatted.substr(4,formatted.size());
    // convert into ascii chars
    size_t x = 0;
    while (x < formatted.size()) {
        output += hexToChar(formatted.substr(x, 2));
        x += 2;
    }
    return output;
}
// converts single hex char pair to an ascii char
char ObdSerial::hexToChar(std::string input) {
    if (input.size() != 2) {
        std::cerr << "Wrong sized string passed to hexToChar" << std::endl;
        throw "Invalid hex string to convert to ASCII in hexToChar";
    }
    std::stringstream ss;
    ss << std::hex << input;
    int x;
    ss >> x;
    return (char) (x);
}
// converts hex string into ABCD data interpretation format
ObdSerial::OBDDatum ObdSerial::hexStrToABCD(std::string & input) {
    OBDDatum info;
    std::string byte;
    if ( ((input.size() % 2) != 0) || (input.size() > 8) ) {
        std::cerr << "Wrong sized string passed to hexStrToABCD" << std::endl;
        info.error = true;
        return info;
    }
    for (unsigned int x = 0; x < input.size() / 2; x++) {
        byte = input.substr(2*x, 2);
        info.abcd[x] = (int) strtol(byte.c_str(), NULL, 16);
        byte = "";
    }
    info.error = false;
    return info;
}
// -1 for failure 0 for success
int ObdSerial::writeToOBD(size_t cmdindex) {
    char outstr[20];
    int outslen;
    if (cmdindex > 100 /*size of obdcmds_mode1*/) {
        std::cerr << "Error in writeToOBD - cmd not in obdcmds_mode1" << std::endl;
        return -1;
    }
    outslen = snprintf(outstr, sizeof(outstr), "%02X%s%02X%01d\r", 0x01, " ", obdcmds_mode1[cmdindex].cmdid, obdcmds_mode1[cmdindex].bytes_returned);
    if (write(fd, outstr, outslen) < outslen) {
        std::cerr << "Error in writeToOBD for cmd " << obdcmds_mode1[cmdindex].cmdid << std::endl;
        return -1;
    }
    return 0;
}
// trim read to one line of return chars with no echo tags
// returns 0 for success -1 for failure
int ObdSerial::readFromOBD(std::string & strtoreadto) {
    char inbuf[4096];
    // read into inbuf, put it into a string, take first line with getline
    read(fd, inbuf, sizeof(inbuf));
    strtoreadto = inbuf;
    std::istringstream iss(strtoreadto);
    std::getline(iss, strtoreadto);
    if ( (strtoreadto == "NO DATA") || (strtoreadto == "?") || (strtoreadto == "STOPPED") || (strtoreadto == "ERROR") ) {
        std::cerr << "Bad data read from OBD port: " << strtoreadto << std::endl;
        return -1;
    }
    strtoreadto = strtoreadto.substr(4, strtoreadto.size());
    return 0;
}

/// Static method to poll for data in its own thread (called by public member function "start()")
// turned off from outside the object by setting ObdSerial instance variable "boolrun" to false
void ObdSerial::run() {
    OBDDatum datum;
    double dataValue;
    while (boolrun == true) {
        for (unsigned int ind = 0; ind < focusPIDs.size(); ind++) {
            if (boolrun==false) {
                continue;
            }
            if (writeToOBD(focusPIDs.at(ind)) != 0) {
                continue;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(NORMAL_OBD_SLEEPTIME));
            std::string strtoreadto;
            if (readFromOBD(strtoreadto) != 0) {
                continue;
            }
            datum = hexStrToABCD(strtoreadto);
            if (datum.error == false) {
                dataValue = obdcmds_mode1[focusPIDs.at(ind)].conv(datum.abcd[0], datum.abcd[1], datum.abcd[2], datum.abcd[3]);
            }

            //std::cout << obdcmds_mode1[focusPIDs.at(ind)].human_name << " = " << dataValue << std::endl;

            // convert dataValue to string, and send it off to observers
            std::string maxval = std::to_string( (int) obdcmds_mode1[focusPIDs.at(ind)].max_value);
            std::string minval = std::to_string( (int) obdcmds_mode1[focusPIDs.at(ind)].min_value);
            size_t maxlen = (maxval.size() > minval.size() ? maxval.size() : minval.size());
            std::string output = std::to_string(dataValue).substr(0, maxlen);
            notifyObservers(focusPIDs.at(ind), output);

        } //end for
    } //end while
}
