#ifndef IOHANDLER_H
#define IOHANDLER_H

//#include <string>
#include "Observer.h"
#include "Controller.h"

//view layer, spawned by controller object

class Controller;

class IOHandler: public Observer
{
    public:
        IOHandler(int bleft, int bright, int bsel,                  //these are the three button GPIO pin numbers
                  int rs, int strb, int d0, int d1,                 //and these are the GPIO pins for the LCD, in 8-bits
                  int d2, int d3, int d4, int d5, int d6, int d7, Controller * cont);
        void moveCursor(int spot);
        void update();
        int getCurPos();
    protected:
        void printScreen(std::string text);
    private:
        unsigned int cursorPosition;
        int LCDHandle;
        Controller * controller;
};

#endif // IOHANDLER_H
