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
/// 1) write update()

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
    std::lock_guard<std::mutex> locker(print_lock);
    lcdPosition(LCDHandle, spot%20,spot/20);
    lcdPuts(LCDHandle, text.c_str());
    lcdPosition(LCDHandle, cursorPosition%20, cursorPosition/20);
}

void IOHandler::scrollText(int startSpot, int stopSpot, int lineNum, string msg) {
    string message = msg;
    message.append(" ");
    message.append(message);
    string toScreen = message.substr(0, stopSpot-startSpot);

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

void IOHandler::startScrollText(int startSpot, int stopSpot, int lineNum, string& msg) {
    if ( (stopSpot >= startSpot) || (lineNum > 3) || (lineNum < 0) || (msg.size() < (stopSpot-startSpot)) ) {
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
    if ( (lineNum <= 3) && (lineNum >= 0) ) {
        lineThreadBools[lineNum-1] = false;
    }
}

void IOHandler::update(int linenum, string info) {
    /// prints "info" at curPage's offset on "linenum"
    /// take current page's "updates are left justified" into account
}

void IOHandler::printPage(ScreenData& screendata) {

    // recieves a screendata object from the controller and prints it to the screen

    string line0;
    string contentLines[3];
    int endScrollSpot;

    line0 = screendata.getTitle();
    if (line0.size() > 15) {
        line0 = line0.substr(0,15);
    }
    while (line0.size() < 15) {
        line0.append(" ");
    }
    line0.append("  <->");
    printToLCD(line0, 0);

    if (screendata.islabeled==false) {
        for (int contentLineNum = 0; contentLineNum < 3; contentLineNum++) {
            contentLines[contentLineNum] = screendata.getTextForLine(contentLineNum);
            if (contentLines[contentLineNum].size() > 20) {
                startScrollText(0, 19, contentLineNum, contentLines[contentLineNum]);
            }
            else {
                printToLCD(contentLines[contentLineNum], 20 + 20*contentLineNum);
            }
            updateSpotForLine[contentLineNum] = 0;
        }
    }
    else { // this is for when we're printing a labeledScreenData page
        for (int contentLineNum = 0; contentLineNum < 3; contentLineNum++) {
            contentLines[contentLineNum] = screendata.getTextForLine(contentLineNum);

            //if the scrollable text doesn't fit in the remaining line space, scroll it
            if (contentLines[contentLineNum].size() > (20 - lineLbls[contentLineNum].size() - maxLengthOfDataOnLine[contentLineNum]) ) {

                // set up scrolling part of line
                endScrollSpot = 20 - lineLbls[contentLineNum].size() - maxLengthOfDataOnLine[contentLineNum] - 1 /*for 0 counting*/ - 1 /*for a space*/;
                updateSpotForLine[contentLineNum] = 20-lineLbls[contentLineNum].size()-1;
                startScrollText(0, endScrollSpot, contentLineNum, contentLines[contentLineNum]);

                // print rest of line as static
                contentLines[contentLineNum] = "";
                contentLines[contentLineNum].append(" ");
                for (int spaces = 0; spaces < maxLengthOfDataOnLine[contentLineNum]; spaces++) {
                    contentLines[contentLineNum].append(" "); // add spaces for data field
                }
                contentLines[contentLineNum].append(lineLbls[contentLineNum]);

                printToLCD(contentLines[contentLineNum], endScrollSpot + 1);
            }
            else { // if it does fit, print a static line
                while ( contentLines[contentLineNum].size() < ( 20-lineLbls[contentLineNum].size() ) ) {
                    contentLines[contentLineNum].append(" ");
                }
                updateSpotForLine[contentLineNum] = 20-lineLbls[contentLineNum].size()-1;
                contentLines[contentLineNum].append(lineLbls[contentLineNum]);
                printToLCD(contentLines[contentLineNum], 20 + 20*contentLineNum);
            }
        }
    }
}

