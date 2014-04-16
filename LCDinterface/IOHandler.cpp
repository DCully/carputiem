#include <wiringPi.h>
#include <lcd.h>
#include "IOHandler.h"
#include <string>
#include "../Observer.h"
#include <cstdlib>
#include <sstream>
#include <iostream>
#include <thread>
#include <chrono>
#include "Controller.h"
// #include "../unit_tests/wPi_mock.h"

using namespace std;

IOHandler::IOHandler(const int& bleft, const int& bright, const int& bsel,
                     const int& rs, const int& strb, const int& d0, const int& d1,
                     const int& d2, const int& d3, const int& d4, const int& d5,
                     const int& d6, const int& d7, Controller * cont)
{
    wiringPiSetup();
    LCDHandle = lcdInit(4,20,8, rs,strb, d0,d1,d2,d3,d4,d5,d6,d7);
    wiringPiISR(bleft, INT_EDGE_FALLING, &Controller::lButPressed);
    wiringPiISR(bright, INT_EDGE_FALLING, &Controller::rButPressed);
    wiringPiISR(bsel, INT_EDGE_FALLING, &Controller::selPressed);

    lcdCursor(LCDHandle, 1); // experiment with cursor turned off
    lcdCursorBlink(LCDHandle, 0); // dont blink the cursor
    moveCursor(cont->getCurPage()->getCurrentCursorSpot());
    controller = cont;
}

void IOHandler::moveCursor(const int& spot) {
    if (spot > 79 || spot < 0) {
        cerr << "Invalid cursor spot passed to moveCursor" << endl;
        //throws "Invalid cursor spot passed to moveCursor";
    }

    std::lock_guard<std::mutex> locker2(cursor_lock);
    lcdPosition(LCDHandle, spot%20, spot/20);

}

void IOHandler::update(size_t linenum, string info) {
    controller->getCurPage()->getLineSetupBehavior()->updateLine(this, linenum, info);
}

void IOHandler::printToLCD(const string& text, const int& spot) {

    // ensures that no other thread prints in middle of this print
    std::lock_guard<std::mutex> locker(print_lock);

    // ensures that nothing else can moveCursor in middle of this print
    std::lock_guard<std::mutex> locker2(cursor_lock);

    lcdPosition(LCDHandle, spot%20,spot/20);
    lcdPuts(LCDHandle, text.c_str());

    /// do we need getCurrentCursorSpot() ?
    lcdPosition(LCDHandle, controller->getCurPage()->getCurrentCursorSpot()%20, controller->getCurPage()->getCurrentCursorSpot()/20);
}

void IOHandler::startScrollText(const std::vector<size_t>& startSpots,
    const std::vector<size_t>& stopSpots,
    const std::vector<size_t>& lineNums,
    const std::vector<std::string>& msgs)
{
    // check input validity for all scrolled lines
    for (size_t i = 0; i < lineNums.size(); i++) {
        if (lineNums.size() != stopSpots.size() || lineNums.size() != startSpots.size() || lineNums.size() != msgs.size()) {
            cerr << "Error in startScrollText - vector sizes are uneven" << endl;
            return ;
        }
        if ( stopSpots.at(i) <= startSpots.at(i) ) {
            cerr << "Error in startScrollText - starting spot was before stopping spot for line " << lineNums.at(i) << endl;
            return ;
        }
        if (  (lineNums.at(i) > 3) || (lineNums.at(i) < 0)  ) {
            cerr << "Error in startScrollText - invalid line number: " << lineNums.at(i) << endl;
            return ;
        }
        if (stopSpots.at(i)/20 != startSpots.at(i)/20) {
            cerr << "Error in startScrollText - can't scroll across multiple lines" << endl;
            return ;
        }
        if ( msgs.at(i).size() < ( stopSpots.at(i) - startSpots.at(i) ) ) {
            cerr << "Error in startScrollText - message size less than scrolling area" << endl;
            return ;
        }
    }

    // double check to make sure nothing's scrolling
    if (TextIsScrolling) {
        stopAllScrollingText();
    }

    // launch a new scroll manager thread
    TextIsScrolling = true;
    ScrollingThread = new std::thread(&IOHandler::textScroller, this, startSpots, stopSpots, lineNums, msgs);

}

void IOHandler::stopAllScrollingText() {
    TextIsScrolling = false;
    ScrollingThread->join();
}

void IOHandler::textScroller(std::vector<size_t> startSpots,
    std::vector<size_t> stopSpots,
    std::vector<size_t> lineNums,
    std::vector<std::string> msgs)
{
    // these only become as large as however many lines are actually scrolled (not always three)
    vector<string> toScreen;
    vector<size_t> spotInMsgs;
    unsigned int lastPrint = 0;

    for (size_t x = 0; x < msgs.size(); ) {
        msgs.at(x).append(" ");
        msgs.at(x).append(msgs.at(x));
        toScreen.push_back(msgs.at(x).substr(0, stopSpots.at(x) - startSpots.at(x) + 1));
        spotInMsgs.push_back(0);
    }

    while (TextIsScrolling) {
        if (millis() - lastPrint > 200) {
            for (size_t i = 0; i < msgs.size(); i++) {
                printToLCD(toScreen.at(i), lineNums.at(i)*20 + startSpots.at(i));
                spotInMsgs.at(i) = (spotInMsgs.at(i) + 1) % (msgs.at(i).size()/2);
                toScreen.at(i) = msgs.at(i).substr(spotInMsgs.at(i), toScreen.at(i).size());
            }
            lastPrint = millis();
        }
    }
}


