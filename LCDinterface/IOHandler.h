#ifndef IOHANDLER_H
#define IOHANDLER_H

#include <string>
#include "../Observer.h"
#include "Controller.h"

class Controller;

class IOHandler: public Observer
{
    public:
        IOHandler(int bleft, int bright, int bsel,                  //these are the three button GPIO pin numbers
                  int rs, int strb, int d0, int d1,                 //and these are the GPIO pins for the LCD, in 8-bits
                  int d2, int d3, int d4, int d5, int d6, int d7, Controller * cont);
        void update(int linenum, std::string infoToPrint); // only called by model layer objects

        // these are the mechanisms by which the controller executes its decisions
        int getCurPos();
        void printToLCD(std::string text, int spot);
        void moveCursor(int spot);
        void startScrollText(int startSpot, int stopSpot, int lineNum, std::string& msg);
    private:
        unsigned int cursorPosition;
        void scrollText(int startSpot, int stopSpot, int lineNum, std::string& msg);
        int LCDHandle;
        Controller * controller;

        // three threads for three scrolling lines
        pthread_t line1thread;
        pthread_t line2thread;
        pthread_t line3thread;
        volatile bool lineThreadBools[3];
};

#endif // IOHANDLER_H
