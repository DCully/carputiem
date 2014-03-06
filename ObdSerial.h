#ifndef OBDSERIAL_H
#define OBDSERIAL_H
#include <string>
#include <termios.h>
#include <vector>
#include "Observable.h"
#include "obdcomms.h"
class ObdSerial: public Observable
{
    public:
        ObdSerial(std::string pp);
        ~ObdSerial();
        void start();
        std::string getOBDDataLbl(size_t index);
        std::string getOBDDataName(size_t index);
        double getOBDDataPoint(size_t index);
        struct OBDDatum {
            unsigned int abcd[4];
            bool error;
        };
        void test();
    private:
        OBDDatum* parseOBDLine(char* buffer, size_t cmdindex); // helper function, mostly for writeToOBD
        int writeToOBD(size_t cmdindex);
        OBDDatum* readFromOBD(size_t cmdindex); // handles read call
        void updateDatum(size_t dnum, double datum); // updates the vector holding the current OBD data
        std::string hexToBin(char x);
        int hexToInt(std::string input);
        int fd; // handle for the obd's serial port connection
        struct termios options; // config struct for the serial port
        std::vector<std::string> obdDataNames; // human names of each datum
        std::vector<double> obdDataPoints;     // actual values for each datum
        std::vector<std::string> obdDataLbls;  // units for each datum
        std::vector<int> supCmdsInfo; // all of the supported command indexes, as found by ctor
        int * cmds; // prioritized list of commands, built by constructor (has duplicates)
        size_t cmdscount;

        int counter;
};
#endif //OBDSERIAL_H
