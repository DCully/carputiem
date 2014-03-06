#include "ObdSerial.h"
#include <fcntl.h>
#include <termios.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include "obdcomms.h" // pid table
#include "obdconvs.h" // conversion functions
#include <stdio.h>
#include <cstring>
#include <cstdlib>
#include <sstream>

using namespace std;

/// Problem: it seems like some of the A, B, C, D values get calculated wrong

    /// TODO:
    /// 0) Build cmds[] to prioritize certain PIDs
    /// 1) Build this into its own thread (add multithreading with POSIX for portability)
    /// 2) Add better error handling everywhere
    /// 3) Make the code less fragile (reliant on specific OBD output formats, etc)
    /// 4) Add support for PIDs farther down the list in obdcmds_mode1
    /// 5) Add ELM327 optimizations
    /// 6) Add support for bitwise encoded PIDs

ObdSerial::ObdSerial(string portpath) {
    //open serial port connection
    char buf[4096];
    counter = 0;
    fd = open(portpath.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1) {
        cerr << "failed to open obd port..." << endl;
    }
    else {
        // configure the opened serial port
		fcntl(fd, F_SETFL, 0);
		tcgetattr(fd, &options);
        options.c_cflag |= (CLOCAL | CREAD);
		options.c_lflag &= !(ICANON | ECHO | ECHOE | ISIG);
		options.c_oflag &= !(OPOST);
		options.c_cc[VMIN] = 0;         // read doesn't block... takes whatever it gets
		options.c_cc[VTIME] = 20;       // 5 second read timeout (what should this be?)
		cfsetispeed(&options, B38400);
		cfsetospeed(&options, B38400);
        // set those attributes for the port
		if (tcsetattr(fd, TCSANOW, &options) != 0) {
            cerr << "Error setting serial port attributes" << endl;
		}
        write(fd, "AT Z\r", sizeof("AT Z\r")); //reset device
        usleep(10000);                      // these seem necessary to process AT comms
        read(fd, buf, sizeof(buf));         // you need these reads to clear the buffer in ELM327
        write(fd, "AT E0\r", sizeof("AT E0\r")); // stop command echo
        usleep(10000);
        read(fd, buf, sizeof(buf));
        write(fd, "AT L0\r", sizeof("AT L0\r")); // disable linefeeds
        usleep(10000);
        read(fd, buf, sizeof(buf));
        write(fd, "AT S0\r", sizeof("AT H1\r")); //turn off spaces - definitely does stuff
        usleep(10000);
        read(fd, buf, sizeof(buf));

        // get the car's capabilities (query port with PIDs 00, 20, 40, and interpret)
        int getcmds[3] = {0x00, 0x20, 0x40};
        char outstring[20];
        string binary;

        for (int x = 0; x < 3; x++) {
            string retstr;
            write(fd, "AT H1\r", sizeof("AT H1\r")); //turn on headers to help with multiple ECU responses
            usleep(10000);
            read(fd, buf, sizeof(buf));
            snprintf(outstring, sizeof(outstring), "%02X%s%02X\r", 0x01, " ", getcmds[x]);
            write(fd, outstring, sizeof(outstring));
            usleep(300000); // needs to be higher this time for some reason
            read(fd, buf, sizeof(buf));
            retstr = buf;

            // could add check for correct PID response here

            if (retstr.substr(6,5) != "7E806") { //if the first output isn't from engine ECU, discard it
                retstr = retstr.substr(33,8);
            }
            else { // the first response IS the engine - always choose engine
                retstr = retstr.substr(15,8);
            }

            if (retstr.size() != 8) { //got it wrong
                cerr << "Error interpreting supp'd PIDs response" << endl;
            }
            // convert "XXXXXXXX" to binary string
            string bits;
            string nextbits;
            for (int j = 0; j < 8; j++) {
                nextbits = hexToBin(retstr.at(j));
                // need to add error handling with ERROR return here...
                bits += nextbits;
            }

            // loop over the binary string and add necessary PIDs to supCmdsInfo
            for (unsigned int i = 0; i < bits.size(); i++) {
                if (bits.at(i)=='1' && (i+32*x < 83)) { //this PID is supported
                    supCmdsInfo.push_back(i+32*x);
                    cout << obdcmds_mode1[i+32*x].human_name << endl;
                }
            }

        } // end outer for loop

        /// TODO: prioritize PIDs by building cmds[] better
        cmdscount = supCmdsInfo.size();
        cmds = new int[cmdscount];
        // build obdDataPoints, obdDataLbls, and obdDataNames
        for (unsigned int x = 0; x < cmdscount; x++) { //for every supported PID...
            if (supCmdsInfo.at(x)<83) { // struct isnt built that far out yet...
                obdDataLbls.push_back(obdcmds_mode1[supCmdsInfo.at(x)].units);
                obdDataNames.push_back(obdcmds_mode1[supCmdsInfo.at(x)].human_name);
                obdDataPoints.push_back(0); //build data vector with 0 in every slot (update later)
                cmds[x] = supCmdsInfo.at(x); // put all the supported cmd rows into cmds[]
            }
        }
    } // end else
    write(fd, "AT H0\r", sizeof("AT H0\r")); //turn off headers
    usleep(10000);
    read(fd, buf, sizeof(buf));
} // end constructor
ObdSerial::~ObdSerial() {
    //reset device and close serial port
    char buf[4096];
    write(fd, "AT Z\r\0", sizeof("AT Z\r\0"));
    usleep(300000);
    read(fd, buf, sizeof(buf));
    close(fd);
    delete cmds;
}
int ObdSerial::hexToInt(string input) { //returns -1 on error
    int out;
    char x;
    int holdval;
    int exp;
    if (input.size() != 2) {
        cerr << "Wrong size string passed into hexToInt, returning -1" << endl;
        return -1;
    }

    // given "AB", out = A*16^1 + B*16^0, and "A" = input[0], B=input[1]
    for (int c=0;c<2;c++) {
        // get decimal equivalent
        x = input[c];
        if (x == '0') {
            holdval=0;
        }
        else if (x == '1') {
            holdval=1;
        }
        else if (x == '2') {
            holdval=2;
        }
        else if (x == '3') {
            holdval=3;
        }
        else if (x == '4') {
            holdval=4;
        }
        else if (x == '5') {
            holdval=5;
        }
        else if (x == '6') {
            holdval=6;
        }
        else if (x == '7') {
            holdval=7;
        }
        else if (x == '8') {
            holdval=8;
        }
        else if (x == '9') {
            holdval=9;
        }
        else if (x == 'A') {
            holdval=10;
        }
        else if (x == 'B') {
            holdval=11;
        }
        else if (x == 'C') {
            holdval=12;
        }
        else if (x == 'D') {
            holdval=13;
        }
        else if (x == 'E') {
            holdval=14;
        }
        else if (x == 'F') {
            holdval=15;
        }
        else {
            cerr << "Wrong type of character passed to hexToInt, returning -1" << endl;
            return -1;
        }
        out += (c==0 ? holdval*16 : holdval);
    }
    return out;
}
string ObdSerial::hexToBin(char x) {
    // don't you judge me now
    if (x == '0') {
        return "0000";
    }
    else if (x == '1') {
        return "0001";
    }
    else if (x == '2') {
        return "0010";
    }
    else if (x == '3') {
        return "0011";
    }
    else if (x == '4') {
        return "0100";
    }
    else if (x == '5') {
        return "0101";
    }
    else if (x == '6') {
        return "0110";
    }
    else if (x == '7') {
        return "0111";
    }
    else if (x == '8') {
        return "1000";
    }
    else if (x == '9') {
        return "1001";
    }
    else if (x == 'A') {
        return "1010";
    }
    else if (x == 'B') {
        return "1011";
    }
    else if (x == 'C') {
        return "1100";
    }
    else if (x == 'D') {
        return "1101";
    }
    else if (x == 'E') {
        return "1110";
    }
    else if (x == 'F') {
        return "1111";
    }
    else {
        cerr << "Error in hexToBin, returning ERROR" << endl;
        return "ERROR";
    }
}
string ObdSerial::getOBDDataLbl(size_t index) {
    return obdDataLbls.at(index);
}
string ObdSerial::getOBDDataName(size_t index) {
    return obdDataNames.at(index);
}
double ObdSerial::getOBDDataPoint(size_t index) {
    return obdDataPoints.at(index);
}
int ObdSerial::writeToOBD(size_t cmdindex) { // return 0 for success -1 for error
    char outstr[20];
    int outslen;
    outslen = snprintf(outstr, sizeof(outstr), "%02X%s%02X%01d\r", 0x01, " ", obdcmds_mode1[cmds[cmdindex]].cmdid, obdcmds_mode1[cmds[cmdindex]].bytes_returned);
    if (write(fd, outstr, outslen) < outslen) {
        cerr << "Error writing to serial port for cmd " << obdcmds_mode1[cmds[cmdindex]].cmdid << endl;
        return -1;
    }
    return 0;
}
ObdSerial::OBDDatum* ObdSerial::readFromOBD(size_t cmdindex) {
    OBDDatum * parsed;
    char inbuf[4096];
    read(fd, inbuf, sizeof(inbuf)); // timeout built into read() : VTIME
    // pass buffer onward if read was successful
    parsed = parseOBDLine(inbuf, cmdindex);
    return parsed;
}
ObdSerial::OBDDatum* ObdSerial::parseOBDLine(char* buffer, size_t cmdindex) {
    OBDDatum * parseddata = new OBDDatum();
    int byte; // output from hexToInt
    string buf = buffer; // get raw response into a string
    buf = buf.substr(0, buf.find_first_of(">")); // lop off after prompt char
    buf = buf.substr(7, buf.size()); // remove cmd signature
    istringstream iss(buf);
    getline(iss, buf);
    iss >> buf; // get only the first line of the string
    buf = buf.substr(4, buf.size()); // lop of the echoed "41XX"

    if (buf=="ATA") {
        parseddata->error=true;
        //cerr << "NO DATA: " << obdcmds_mode1[cmds[cmdindex]].human_name << endl;
        counter++;
        return parseddata;
    }
    int bytenums = obdcmds_mode1[cmds[cmdindex]].bytes_returned;

    //go bytenums times thru loop
    string send = "";
    int y = 0;
    for (int x = bytenums; x>0; x--) {
        send += buf.at(2*y);
        send += buf.at(2*y + 1);
        y++;
        byte = hexToInt(send);
        if (byte == -1) {
            parseddata->error = true;
            return parseddata;
        }
        parseddata->abcd[y] = byte;
        send = "";
    }
    parseddata->error = false;
    return parseddata;
}
void ObdSerial::updateDatum(size_t datumIndex, double datum) {
    obdDataPoints.at(datumIndex) = datum;
}


void ObdSerial::start() {
    OBDDatum * datum;
    double updatedDatum;
//    while (true) {
        for (size_t i = 0; i < supCmdsInfo.size(); i++) {
            if (obdcmds_mode1[cmds[i]].conv == NULL) {
                //cout << "Bitwise Encoded, skipped: " << obdcmds_mode1[cmds[i]].human_name << endl;
                counter++;
                continue;
            }
            if (writeToOBD(i) == 0) {
                //cout << obdcmds_mode1[cmds[i]].human_name << endl;
                // sleep 1/10th second before reading to let car catch up (minimum allowed)
                usleep(500000);
                datum = readFromOBD(i);
                if (datum->error == false) { // only updates on successful data pull
                    updatedDatum = obdcmds_mode1[cmds[i]].conv(datum->abcd[0], datum->abcd[1], datum->abcd[2], datum->abcd[3]);
                    updateDatum(i, updatedDatum);
                    counter++;
                    //cout << "A: " << datum->abcd[0] << "  B: " << datum->abcd[1] << "  C: " << datum->abcd[2] << "  D: " << datum->abcd[3] << endl;
                   // cout << "Successful: " << obdDataNames.at(i) << ": " << obdDataPoints.at(i) << obdDataLbls.at(i) << endl;
                }
                else {
                    //cerr << "Error getting " << obdcmds_mode1[cmds[i]].human_name << endl;
                    continue;
                }
                delete datum;
            }
        } //end for loop
           // cout << endl;
           // cout << "Total number of PIDs found: " << cmdscount << endl;
           // cout << "Total number of PIDs queried: " << counter << endl;
//    } // end while loop
}
