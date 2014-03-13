#ifndef OBDSERIAL_H
#define OBDSERIAL_H
#include <string>
#include <termios.h>
#include <vector>
#include "../Observable.h"
#include "obdcomms.h"
#include <map>
#include <pthread.h>

class ObdSerial: public Observable
{
    friend class ObdSerialTest;

    public:
        ObdSerial(std::string pp);
        ~ObdSerial();
        void start(); // called to spawn the data polling thread
        /// need accessor functions
    private:
        struct OBDDatum {
            unsigned int abcd[4];
            bool error;
        };
        int fd; // serial port handle
        struct termios options; // config struct for the serial port
        const int AT_SLEEPTIME;
        const int NORMAL_OBD_SLEEPTIME; // sleep time for normal PIDs
        const int GETPIDS_OBD_SLEEPTIME; // sleep time for PIDs 00, 20, 40, 60
        std::map<size_t, double> suppdCmds; // maps index of an element in obdcmds_mode1 to its current value
        std::string VIN;
        void getVIN();
        ObdSerial::OBDDatum hexStrToABCD(std::string& input); // puts a hex string into an OBDDatum
        int fillSuppdCmds(); // interprets PIDs 00, 20 40, 60
        int writeToOBD(size_t cmdindex);
        int readFromOBD(std::string& stringtoreadto); // handles read call and gets the correct line

        // multithreading
        bool boolrun;
        pthread_t thread;
        static void* run(void* waste);
};
#endif //OBDSERIAL_H
