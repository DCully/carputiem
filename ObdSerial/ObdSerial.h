#ifndef OBDSERIAL_H
#define OBDSERIAL_H
#include <string>
#include <termios.h>
#include <vector>
#include "../Observable.h"
#include "obdcomms.h"
#include <map>
#include <thread>
#include <mutex>

class badSerialConnectException : public std::exception
{
    virtual const char* what() const throw()
    {
        return "Serial port failed to connect in ObdSerial";
    }
};

/*
* This class sets up a serial connection with the car and polls for
* whatever data you tell it to, looping through a list of PIDs that you
* set with setFocusedPIDs. The numbers in the inputted vector must correspond
* to supported PIDs as indexed by the obdcmds_mode1 struct.
*/
class ObdSerial: public Observable
{
    public:
        ObdSerial(const std::string& pp);
        ~ObdSerial();

        void start(); // spawns the data polling thread
        std::vector<int> getSuppdCmds();
        const std::string& getVIN();
        void setFocusedPIDs(const std::vector<int>& focusPIDs); // sets which PIDs to query for

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
        static std::mutex obdLock;
        static volatile bool boolrun;
        void run();
};
#endif //OBDSERIAL_H
