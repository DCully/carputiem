#include <wiringPi.h>
#include <lcd.h>
#include "IOHandler.h"
#include <string>
#include "../Observer.h"
#include <cstdlib>
#include <sstream>
#include <iostream>
#include <thread>
#include "Controller.h"
// #include "../unit_tests/wPi_mock.h"

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
    cursorPosition = 19;
    moveCursor(cursorPosition);
    controller = cont;
}

void IOHandler::moveCursor(int spot) {
    if (spot > 79 || spot < 0) {
        cerr << "Invalid cursor spot passed to moveCursor" << endl;
        //throws "Invalid cursor spot passed to moveCursor";
    }
    cursorPosition = spot;
    lcdPosition(LCDHandle, spot%20, spot/20);
}

void IOHandler::printToLCD(string text, int spot) {
    std::lock_guard<std::mutex> locker(print_lock);
    lcdPosition(LCDHandle, spot%20,spot/20);
    lcdPuts(LCDHandle, text.c_str());
    lcdPosition(LCDHandle, cursorPosition%20, cursorPosition/20);
}

void IOHandler::scrollText(int startSpot, int stopSpot, int lineNum, string msg) {
    string message = msg;
    message.append(" ");
    message.append(message);
    string toScreen = message.substr(0, stopSpot-startSpot+1);

    while (lineThreadBools[lineNum-1]==true) {
        for (size_t spotInMessage = 0; spotInMessage < message.size()/2; spotInMessage++ ) {
            if (lineThreadBools[lineNum-1]==false) {
                break;
            }
            printToLCD(toScreen, lineNum*20+startSpot);
            toScreen = message.substr(spotInMessage, toScreen.size());
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
    }
}

void IOHandler::startScrollText(int startSpot, int stopSpot, int lineNum, string msg) {
    if ( (stopSpot >= startSpot) || (lineNum >= 3) || (lineNum < 0) || (msg.size() < (stopSpot-startSpot)) ) {
        cerr << "Invalid parameters passed to startScrollText" << endl;
        return ;
    }
    if (stopSpot/20 != startSpot/20) {
        cerr << "Error in startScrollText - can't scroll across multiple lines" << endl;
        return ;
    }
    if (lineThreadBools[lineNum-1]==true) {
        cerr << "Error in startScrollText - previous text scrolling thread still running" << endl;
        return ;
    }
    lineThreadBools[lineNum-1]==true;
    std::thread scrollthread (&IOHandler::scrollText, this, startSpot, stopSpot, lineNum, msg);
    scrollthread.join();
}

void IOHandler::stopScrollTextOnLine(int lineNum) {
    if ( (lineNum < 3) && (lineNum >= 0) ) {
        lineThreadBools[lineNum-1] = false;
    }
}

void IOHandler::update(size_t linenum, string info) {
    controller->getCurPage()->getLineSetupBehavior()->updateLine(this, linenum, info);
}

// recieves a screendata object from the controller and prints it to the screen
void IOHandler::printPage(ScreenData& curPage) {

    for (int line = 0; line < 3; line++) {
        curPage.getLineSetupBehavior()->renderLine(this, line);
    }

}
