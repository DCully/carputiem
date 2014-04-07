#ifndef IOHANDLER_H
#define IOHANDLER_H

#include <string>
#include "../Observer.h"
#include <mutex>
#include "ScreenData.h"

class Controller;
class ScreenData;

class IOHandler: public Observer
{

    public:
        IOHandler(const int& bleft, const int& bright, const int& bsel,                       //these are the three button GPIO pin numbers,
                  const int& rs, const int& strb, const int& d0, const int& d1,               //and these are the GPIO pins for the LCD (8-bit)
                  const int& d2, const int& d3, const int& d4, const int& d5,
                  const int& d6, const int& d7, Controller * cont);

        // for observable object - prints to appropriate update location for linenum
        void update(size_t linenum, std::string infoToPrint);

        // for controller object
        void printPage(ScreenData& screendata);
        void moveCursor(const int& spot);

        // for LineSetupBehavior objects
        void startScrollText(const int& startSpot, const int& stopSpot, const int& lineNum, const std::string& msg);
        void stopScrollTextOnLine(const int& lineNum);
        void printToLCD(const std::string& text, const int& spot);

    private:
        unsigned int cursorPosition;
        void scrollText(int startSpot, int stopSpot, int lineNum, std::string msg);
        int LCDHandle;
        Controller * controller;
        volatile bool lineThreadBools[4];
        std::mutex print_lock;
};

#endif // IOHANDLER_H
