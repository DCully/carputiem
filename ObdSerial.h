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
            unsigned int a;
            unsigned int b;
            unsigned int c;
            unsigned int d;
            bool error;
        };
        void test();
    protected:
        OBDDatum* parseOBDLine(char* buffer, size_t cmdindex);
        int writeToOBD(size_t cmdindex);
        OBDDatum* readFromOBD(size_t cmdindex); //
        void updateDatum(size_t dnum, double datum); // updates the vector holding the current OBD data
    private:
        int fd; // handle for the obd's serial port connection
        struct termios options; // config struct for the serial port
        std::vector<std::string> obdDataNames; // human names of each datum
        std::vector<double> obdDataPoints;     // actual values for each datum
        std::vector<std::string> obdDataLbls;  // units for each datum
        std::vector<obdservicecmd> supCmdsInfo; // all of the supported commands, as found by ctor
        int * cmds;
        size_t cmdscount;
};
#endif //OBDSERIAL_H
