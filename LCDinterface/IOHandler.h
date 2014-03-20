#ifndef IOHANDLER_H
#define IOHANDLER_H

#include <string>
#include "../Observer.h"
#include "Controller.h"
#include <mutex>
#include "ScreenData.h"

class Controller;

class IOHandler: public Observer
{
    public:
        IOHandler(int bleft, int bright, int bsel,                  //these are the three button GPIO pin numbers
                  int rs, int strb, int d0, int d1,                 //and these are the GPIO pins for the LCD, in 8-bits
                  int d2, int d3, int d4, int d5, int d6, int d7, Controller * cont);
        void update(size_t linenum, std::string infoToPrint);
        void printPage(ScreenData& screendata);
        int getCurPos();
        void moveCursor(int spot);
        void startScrollText(int startSpot, int stopSpot, int lineNum, std::string msg);
        void stopScrollTextOnLine(int lineNum);
    private:
        size_t updateSpotForLine[3]; // used by update method to determine where to print (absolute nums)
        void printToLCD(std::string text, int spot);
        unsigned int cursorPosition;
        void scrollText(int startSpot, int stopSpot, int lineNum, std::string msg);
        int LCDHandle;
        Controller * controller;

        // three flags to terminate three scrolling line threads
        volatile bool lineThreadBools[3];
        std::mutex print_lock;
};

#endif // IOHANDLER_H
