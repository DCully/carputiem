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
        ObdSerial(const std::string& pp);
        ~ObdSerial();

        void start(); // spawns the data polling thread
        std::vector<int> getSuppdCmds();
        std::string getVIN();
        void setFocusedPIDs(const std::vector<int>& focusPIDs); // sets which PIDs to query for
        void setRunStatus(bool brun); // turn data polling thread on or off

    private:

        struct OBDDatum {
            unsigned int abcd[4];
            bool error;
        };
        std::vector<int> focusPIDs; // PIDs to query for repeatedly
        std::vector<int> suppdCmds; // use this instead to store the list of supported PIDs
        std::string VIN; // vehicle identification number
        int fd; // serial port handle
        struct termios options; // config struct for the serial port
        const int AT_SLEEPTIME; // sleep time for talking to the ELM327
        const int NORMAL_OBD_SLEEPTIME; // sleep time for normal PIDs
        const int EXTRA_LONG_SLEEPTIME; // sleep time for PIDs 00, 20, 40, 60

        std::string getVINFromCar();
        ObdSerial::OBDDatum hexStrToABCD(std::string& input); // puts a hex string into an OBDDatum
        int fillSuppdCmds(); // interprets PIDs until the next "PIDs 'X-X+20' supported" PID isn't supported
        char hexToChar(std::string input);

        // these are for mode 1 PID calls only (getVINFromCar==mode9)
        int writeToOBD(size_t cmdindex);
        int readFromOBD(std::string& stringtoreadto); // handles read call and gets the correct line

        // multithreading
        volatile bool boolrun;
        pthread_t thread;
        void run();
};
#endif //OBDSERIAL_H
