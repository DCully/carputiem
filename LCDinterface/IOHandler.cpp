#include <wiringPi.h>
#include <lcd.h>
#include "IOHandler.h"
#include <string>
#include "../Observer.h"
#include <cstdlib>
#include <sstream>
#include <iostream>
#include <thread>

/// TODO:
/// 1) work out and implement update() behavior
/// 2) threaded text scrolling
/// 3) new page printing

using namespace std;

IOHandler::IOHandler(int bleft, int bright, int bsel,               //these are the three button GPIO pin numbers
                  int rs, int strb, int d0, int d1,                 //and these are the GPIO pins for the LCD, in 8-bits
                  int d2, int d3, int d4, int d5, int d6, int d7, Controller * cont)
{
    wiringPiSetup();
    LCDHandle = lcdInit(4,20,8, rs,strb, d0,d1,d2,d3,d4,d5,d6,d7);
    wiringPiISR(bleft, INT_EDGE_FALLING, &Controller::lButPressed);
    wiringPiISR(bright, INT_EDGE_FALLING, &Controller::rButPressed);
    wiringPiISR(bsel, INT_EDGE_FALLING, &Controller::selPressed);

    lcdCursor(LCDHandle, 1);
    lcdCursorBlink(LCDHandle, 1);
    cursorPosition = 18;
    moveCursor(cursorPosition);
    controller = cont;
}

void IOHandler::moveCursor(int spot) {
   cursorPosition = spot;
   lcdPosition(LCDHandle, spot%20, spot/20);
}

int IOHandler::getCurPos() {
    return cursorPosition;
}

void IOHandler::printToLCD(string text, int spot) {
    lcdPosition(LCDHandle, spot%20,spot/20);
    lcdPuts(LCDHandle, text.c_str());
    lcdPosition(LCDHandle, cursorPosition%20, cursorPosition/20);
}
/*
void IOHandler::startScrollText(int startSpot, int stopSpot, int lineNum, string& msg) {
    while
}

void IOHandler::scrollText(int startSpot, int stopSpot, int lineNum, string& msg) {
    if ( (stopSpot >= startSpot) || (lineNum > 3) || (lineNum < 0) ) {
        cerr << "Invalid parameters passed to scrollText" << endl;
        return void;
    }
    string message = msg.substr(0, (stopSpot-startSpot));

}
*/
void IOHandler::update(int linenum, string info) {
    /// prints "info" at curPage's offset on "linenum"
}

