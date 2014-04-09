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

    lcdCursor(LCDHandle, 1);
    lcdCursorBlink(LCDHandle, 1);
    moveCursor(cont->getCurPage()->getCurrentCursorSpot());
    controller = cont;
}

void IOHandler::moveCursor(const int& spot) {
    if (spot > 79 || spot < 0) {
        cerr << "Invalid cursor spot passed to moveCursor" << endl;
        //throws "Invalid cursor spot passed to moveCursor";
    }
    cout << "in movecursor, spot passed was " << spot << endl;

    // this lock ensures that no one can call this while someone else is moving cursor to print
    std::lock_guard<std::mutex> locker2(cursor_lock);
    lcdPosition(LCDHandle, spot%20, spot/20);
}

void IOHandler::update(size_t linenum, string info) {
    controller->getCurPage()->getLineSetupBehavior()->updateLine(this, linenum, info);
}

// recieves a screendata object from the controller and prints it to the screen
void IOHandler::printPage(ScreenData& curPage) {

    for (int line = 0; line < 4; line++) {
        cout << "in printpage, telling linesetupbehavior to print line " << line << endl;
        curPage.getLineSetupBehavior()->renderLine(this, line);
    }

}

void IOHandler::printToLCD(const string& text, const int& spot) {

    // ensures that no other thread prints in middle of this print
    std::lock_guard<std::mutex> locker(print_lock);

    // ensures that nothing else can moveCursor in middle of this print
    std::lock_guard<std::mutex> locker2(cursor_lock);

    lcdPosition(LCDHandle, spot%20,spot/20);
    lcdPuts(LCDHandle, text.c_str());
    lcdPosition(LCDHandle, controller->getCurPage()->getCurrentCursorSpot()%20, controller->getCurPage()->getCurrentCursorSpot()/20);
}

void IOHandler::scrollText(int startSpot, int stopSpot, int lineNum, string msg) {
    string message = msg;
    message.append(" ");
    message.append(message);
    string toScreen = message.substr(0, stopSpot-startSpot);
    size_t spotInMsg = 0;
    unsigned int lastPrint = 0;

    while (lineThreadBools[lineNum]==true) {
        if (millis() - lastPrint > 200) {
            printToLCD(toScreen, lineNum*20+startSpot);
            spotInMsg = (spotInMsg + 1) % (message.size()/2);
            toScreen = message.substr(spotInMsg, toScreen.size());
            lastPrint = millis();
        }
    }
    cout << "exiting scrolltext for line " << lineNum << endl;
}

void IOHandler::startScrollText(const int& startSpot, const int& stopSpot, const int& lineNum, const string& msg) {
    if ( (stopSpot <= startSpot) || (lineNum > 3) || (lineNum < 0) || (msg.size() < (stopSpot-startSpot)) ) {
        cerr << "Invalid parameters passed to startScrollText" << endl;
        return ;
    }
    if (stopSpot/20 != startSpot/20) {
        cerr << "Error in startScrollText - can't scroll across multiple lines" << endl;
        return ;
    }

    // this uses pointers to the scroll threads in order to be able to explicitly destroy the threads

    if (lineThreadBools[lineNum]==true) {
        lineThreadBools[lineNum] = false;
        cout << "set flag to end a thread, waiting for it to join in startScrollText..." << endl;
        lineThreads[lineNum]->join();
        delete lineThreads[lineNum];
    }

    lineThreadBools[lineNum]=true;
    lineThreads[lineNum] = new std::thread(&IOHandler::scrollText, this, startSpot, stopSpot, lineNum, msg);

}

