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

using namespace std;

ObdSerial::ObdSerial(string portpath) : AT_SLEEPTIME(20000), NORMAL_OBD_SLEEPTIME(200000), GETPIDS_OBD_SLEEPTIME(300000) {
    //open serial port connection
    char buf[4096];
    // open to read and write, not as the controlling terminal, and in nonblocking mode
    fd = open(portpath.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1) {
        cerr << "Failed to open obd port..." << endl;
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
        cerr << "Error setting serial port attributes" << endl;
    }
    // configure the ELM327 device
    write(fd, "AT E0\r", sizeof("AT E0\r")); // turn off echo
    usleep(AT_SLEEPTIME);
    read(fd, buf, sizeof(buf));
    write(fd, "AT S0\r", sizeof("AT S0\r")); //turn off spaces
    usleep(AT_SLEEPTIME);
    read(fd, buf, sizeof(buf));
    
    /// lower the timeout settings!

    /*
    What are the various names of the engine ECU in different protocols?

    write(fd, " AT H1\r", sizeof("AT H1\r")); // turns on headers - unnecessary with ECU specified
    usleep(AT_SLEEPTIME);
    read(fd, buf, sizeof(buf));

    write(fd, " AT DPN\r", sizeof("AT DPN\r")); // gets the protocol
    usleep(AT_SLEEPTIME);
    read(fd, buf, sizeof(buf));
    cout << buf << endl;
    */

    write(fd, "AT SH 7E0\r", sizeof("AT SH 7E0\r")); // specify 7E8 device only (engine ECU)
    usleep(AT_SLEEPTIME);
    read(fd, buf, sizeof(buf));

    if (fillSuppdCmds() != 0) {
        cerr << "Error getting supported PIDs" << endl;
        throw "Error getting supported PIDs";
    }
    getVIN();
}
ObdSerial::~ObdSerial() {
    //reset device, stop data query thread, close serial port
    boolrun = false;
    usleep(350000); // make sure run has time to terminate
    char buf[4096];
    write(fd, "AT Z\r\0", sizeof("AT Z\r\0"));
    usleep(AT_SLEEPTIME);
    read(fd, buf, sizeof(buf));
    close(fd);
}
void ObdSerial::start() {
    // start the data harvesting thread ( start() )
    boolrun = true;
    pthread_create(&thread, NULL, &ObdSerial::run, (void*)this); // eww
    pthread_join(thread, NULL);
}
void ObdSerial::getVIN() {
    write(fd, "09\r", sizeof("09\r"));
    sleep(NORMAL_OBD_SLEEPTIME);
    readFromOBD(VIN);
}
// return 0 for success -1 for failure
int ObdSerial::fillSuppdCmds() {
    string instr;
    int cmds[4] = {0, 32, 64, 96}; // indexes of commands in obdcmds_mode1 to get supported PIDs
    for (int x = 0; x < 4; x++) {
        if (writeToOBD(cmds[x]) != 0) {
            return -1;
        }
        usleep(GETPIDS_OBD_SLEEPTIME);

        if (readFromOBD(instr) != 0) {
            return -1;
        }
        /// TODO: add support for cmds 84-100
        unsigned long inStrAsLong = strtoul(instr.c_str(), NULL, 16);
        for (int bit = 0; bit < 32; bit++) { // checks 1-32, then 33-64, etc
            if ( ( (inStrAsLong & (1<<bit)) != 0 ) && ( obdcmds_mode1[32-bit + x*32].conv != NULL ) && ((32-bit + x*32) < 100) ) {
                suppdCmds.insert(std::make_pair(32-bit + x*32, 0.0));
            }
        }
    }
    return 0;
}
// converts hex string into ABCD data interpretation format
ObdSerial::OBDDatum ObdSerial::hexStrToABCD(string & input) {
    OBDDatum info;
    string byte;
    if ( ((input.size() % 2) != 0) || (input.size() > 8) ) {
        cerr << "Wrong sized string passed to hexStrToABCD" << endl;
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
        cerr << "Error in writeToOBD - cmd not in obdcmds_mode1" << endl;
        return -1;
    }
    outslen = snprintf(outstr, sizeof(outstr), "%02X%s%02X%01d\r", 0x01, " ", obdcmds_mode1[cmdindex].cmdid, obdcmds_mode1[cmdindex].bytes_returned);
    if (write(fd, outstr, outslen) < outslen) {
        cerr << "Error in writeToOBD for cmd " << obdcmds_mode1[cmdindex].cmdid << endl;
        return -1;
    }
    return 0;
}
// trim read to one line of return chars with no echo tags
// returns 0 for success -1 for failure
int ObdSerial::readFromOBD(string & strtoreadto) {
    char inbuf[4096];
    // read into inbuf, put it into a string, take first line with getline
    read(fd, inbuf, sizeof(inbuf));
    strtoreadto = inbuf;
    istringstream iss(strtoreadto);
    getline(iss, strtoreadto);
    if ( (strtoreadto == "NO DATA") || (strtoreadto == "?") || (strtoreadto == "STOPPED") || (strtoreadto == "ERROR") ) {
        cerr << "Bad data read from OBD port" << endl;
        return -1;
    }
    strtoreadto = strtoreadto.substr(4, strtoreadto.size());
    return 0;
}

void* ObdSerial::run(void* obdserial) {
    OBDDatum datum;
    ObdSerial * obd = (ObdSerial*) obdserial; // eww
    cout << obd->suppdCmds.size() << endl;
  //  while (obd->boolrun == true) {
    /// add support for high priority PIDs
        for (std::map<size_t,double>::iterator it=obd->suppdCmds.begin(); it!=obd->suppdCmds.end(); it++) {
            if (obd->boolrun==false) {
                continue;
            }
            if (obd->writeToOBD(it->first) != 0) {
                continue;
            }
            usleep(obd->NORMAL_OBD_SLEEPTIME);
            string strtoreadto;
            if (obd->readFromOBD(strtoreadto) != 0) {
                continue;
            }
            datum = obd->hexStrToABCD(strtoreadto);
            if (datum.error == false) {
                it->second = obdcmds_mode1[it->first].conv(datum.abcd[0], datum.abcd[1], datum.abcd[2], datum.abcd[3]); //beautiful
            }
            cout << obdcmds_mode1[it->first].human_name << " = " << it->second << endl;
        } //end for
   // } //end while
   return obdserial;
}



