#ifndef IOHANDLER_H
#define IOHANDLER_H

#include <string>
#include "../Observer.h"
#include <mutex>
#include "ScreenData.h"

class Controller;

class IOHandler: public Observer
{

    friend class LineSetupBehavior;
    friend class LabeledLineSetupBehavior;
    friend class IOHandlerTest;

    public:
        IOHandler(int bleft, int bright, int bsel,                  //these are the three button GPIO pin numbers,
                  int rs, int strb, int d0, int d1,                 //and these are the GPIO pins for the LCD (8-bit)
                  int d2, int d3, int d4, int d5, int d6, int d7, Controller * cont);

        // for observable object
        void update(size_t linenum, std::string infoToPrint);

        // for controller object
        void printPage(ScreenData& screendata);
        void moveCursor(int spot);
    
        // for LineSetupBehavior objects
        void startScrollText(int startSpot, int stopSpot, int lineNum, std::string msg);
        void stopScrollTextOnLine(int lineNum);
        void printToLCD(std::string text, int spot);
        
    private:
        unsigned int cursorPosition;
        void scrollText(int startSpot, int stopSpot, int lineNum, std::string msg);
        int LCDHandle;
        Controller * controller;
        volatile bool lineThreadBools[3];
        std::mutex print_lock;
};

#endif // IOHANDLER_H
