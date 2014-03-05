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


using namespace std;

/// 1) Write parseOBDLine (will OBDDatum struct need more flags for special PIDs?)
/// 2) Use your read/write OBD functions to finish the constructor
/// 3) Build this into its own thread (add multithreading with POSIX for portability)

/// this can't refresh all the PIDs fast enough... need a way to prioritize volatile data points

ObdSerial::ObdSerial(string portpath) {
    //open serial port connection
    fd = open(portpath.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1) {
        cerr << "failed to open obd port..." << endl;
    }
    // needs more serial port opening error handling
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

		char buf[4096];
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

        // get the car's capabilities (query port with PIDs 00, 20, 60, 80 and interpret)
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
            cout << "Wrote: " << outstring << endl;
            usleep(300000); // needs to be higher this time for some reason
            read(fd, buf, sizeof(buf));
            retstr = buf;
/*
            cout << "Read: " << endl;
            cout << buf << endl;
            for (unsigned int y=0; y<strlen(buf); y++) {
                cout << "char " << y << ": " << buf[y] << endl;
            }
*/
            if (retstr.substr(0,6) != outstring) { // make sure it's the right response
                cerr << "Error - serial port response not to given cmd" << endl;
            }
            if (retstr.substr(7,5) != "7E806") { //if the first output isn't from engine ECU
                retstr = retstr.substr(33,8);
            }
            else { // the first response IS the engine
                retstr = retstr.substr(15,8);
            }
            /*
            if (retstr.size() != 8) { //got it wrong
                cerr << "Error interpreting supp'd PIDs response" << endl;
            }
            */
            cout << retstr << endl;

            /// convert "XXXXXXXX" to binary (32 bits, 8/pair)

            /// loop over the binary, and add necessary PIDs to cmds[]

        }

/*
        // 3) build obdDataPoints, obdDataLbls, and obdDataNames
        for (int x = 0; x < cmdscount; x++) { //for every supported PID...
            obdDataLbls.push_back(obdcmds_mode1[cmds[x]].units);
            obdDataNames.push_back(obdcmds_mode1[cmds[x]].human_name);
            obdDataPoints.push_back(0); //build data vector with 0 in every slot (update later)
        }
*/
    }
}
ObdSerial::~ObdSerial() {
    //reset device and close serial port
    char buf[4096];
    write(fd, "AT Z\r\0", sizeof("AT Z\r\0"));
    usleep(300000);
    read(fd, buf, sizeof(buf));
    close(fd);
}



void ObdSerial::test() {
/*    cmds = new int[20];
    cmds[0] = 12; //rpm
    for (int y = 0; y < 9; y++) {
        writeToOBD(0);
        usleep(100000); //this speed works (1/10th second) and is the max allowed by ELM327
        readFromOBD(0);
    }*/
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
    cout << "Wrote: " << outstr << endl;
    if (write(fd, outstr, outslen) < outslen) {
        cerr << "Error writing to serial port for cmd " << obdcmds_mode1[cmds[cmdindex]].cmdid << endl;
        return -1;
    }
    return 0;
}
ObdSerial::OBDDatum* ObdSerial::readFromOBD(size_t cmdindex) {
    OBDDatum * parsed;
    char inbuf[4096];
    int bytesread = 0;
    bytesread = read(fd, inbuf, sizeof(inbuf)); // timeout built into read() : VTIME
    cout << "Read: " << inbuf << endl;
    if (bytesread != obdcmds_mode1[cmds[cmdindex]].bytes_returned) { /// something's wrong here
        //cerr << "Error matching byte size from serial port read on cmdindex " << obdcmds_mode1[cmds[cmdindex]].cmdid << endl;
        parsed = new OBDDatum();
        parsed->error = true;
        return parsed;
    }
    else {
        // pass buffer onward if read was successful
        // parsed = parseOBDLine(buffer, cmdindex);
        parsed = new OBDDatum();
        return parsed;
    }
}
ObdSerial::OBDDatum* ObdSerial::parseOBDLine(char* buffer, size_t cmdindex) {
    OBDDatum * parseddata = new OBDDatum();
    unsigned int A,B,C,D; // fill these by parsing the buffer


    /// TODO: figure out how to split obd output chars into a,b,c,d (depending on cmdindex)


    parseddata->a = A;
    parseddata->b = B;
    parseddata->c = C;
    parseddata->d = D;
    parseddata->error = false;
    return parseddata;
}
void ObdSerial::updateDatum(size_t datumIndex, double datum) {
    obdDataPoints.at(datumIndex) = datum;
}
void ObdSerial::start() {
    OBDDatum * datum;
    double updatedDatum;
    while (true) {
        for (size_t i = 0; i < cmdscount; i++) {
            if (writeToOBD(i) == 0) {
                // sleep 1/10th second before reading to let car catch up (minimum allowed)
                usleep(100000);
                datum = readFromOBD(i);
                if (datum->error == false) {
                    updatedDatum = obdcmds_mode1[cmds[i]].conv(datum->a, datum->b, datum->c, datum->d);
                    updateDatum(i, updatedDatum);
                }
                delete datum;
            } // only updates on success
        }
    }
}
