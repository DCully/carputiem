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

volatile bool IOHandler::run = false;
std::mutex IOHandler::runLock;

IOHandler::IOHandler(const int& bleft, const int& bright, const int& bsel,
                     const int& rs, const int& strb, const int& d0, const int& d1,
                     const int& d2, const int& d3, const int& d4, const int& d5,
                     const int& d6, const int& d7)
{

    wiringPiSetup();
    // this stuff can throw if there's no GPIO connection (like if you're not running this on a Pi)
    LCDHandle = lcdInit(4,20,8, rs,strb, d0,d1,d2,d3,d4,d5,d6,d7);
    if (LCDHandle<0) {
        throw IOHandlerSetupException();
    }
    wiringPiISR(bleft, INT_EDGE_FALLING, &Controller::staticLeftButPressed);
    wiringPiISR(bright, INT_EDGE_FALLING, &Controller::staticRightButPressed);
    wiringPiISR(bsel, INT_EDGE_FALLING, &Controller::staticSelectPressed);

    lcdCursor(LCDHandle, 1); // experiment with cursor turned off
    lcdCursorBlink(LCDHandle, 0); // dont blink the cursor
    moveCursor(17); // cursor starts off on "change page left" spot



    // push a ScrollPacket(true) into the queue and launch thread
    scrollQueue.push(ScrollPacket(true));
    run = true;
    std::cerr<<"launching text scrolling thread"<<std::endl;
    std::thread(&IOHandler::textScroller, this).detach();
}

IOHandler::~IOHandler() {
    std::lock_guard<std::mutex> lock(runLock);
    run = false;
}

// this is how to move the cursor from the outside
void IOHandler::moveCursor(const int& spot) {
    if (spot > 79 || spot < 0) {
        std::cerr << "Invalid cursor spot passed to moveCursor" << std::endl;
    }

    std::lock_guard<std::mutex> locker2(cursor_lock);
    lcdPosition(LCDHandle, spot%20, spot/20);
    cursorSpotOnScreen = spot;
}

void IOHandler::printToLCD(const std::string& text, const int& spot) {

    // ensures that nothing else can moveCursor in middle of this print
    std::lock_guard<std::mutex> locker(cursor_lock);

    lcdPosition(LCDHandle, spot%20,spot/20);
    lcdPuts(LCDHandle, text.c_str());
    lcdPosition(LCDHandle, cursorSpotOnScreen%20, cursorSpotOnScreen/20);
}

// this will push a new ScrollPacket into the queue for the thread to process
// the scrolling thread won't clear the screen! you have to do that
void IOHandler::startScrollText(const std::vector<size_t>& startSpots,
    const std::vector<size_t>& stopSpots,
    const std::vector<size_t>& lineNums,
    const std::vector<std::string>& msgs)
{
    // check input validity for all scrolled lines
    for (size_t i = 0; i < lineNums.size(); i++) {
        if (lineNums.size() != stopSpots.size() || lineNums.size() != startSpots.size() || lineNums.size() != msgs.size()) {
            std::cerr << "Error in startScrollText - vector sizes are uneven" << std::endl;
            return ;
        }
        if ( stopSpots.at(i) <= startSpots.at(i) ) {
            std::cerr << "Error in startScrollText - starting spot was before stopping spot for line " << lineNums.at(i) << std::endl;
            return ;
        }
        if (  (lineNums.at(i) > 3) || (lineNums.at(i) < 0)  ) {
            std::cerr << "Error in startScrollText - invalid line number: " << lineNums.at(i) << std::endl;
            return ;
        }
        if (stopSpots.at(i)/20 != startSpots.at(i)/20) {
            std::cerr << "Error in startScrollText - can't scroll across multiple lines" << std::endl;
            return ;
        }
        if ( msgs.at(i).size() < ( stopSpots.at(i) - startSpots.at(i) ) ) {
            std::cerr << "Error in startScrollText - message size less than scrolling area" << std::endl;
            return ;
        }
    }

    // push a new ScrollPacket
    std::lock_guard<std::mutex> lock(queueLock);
    scrollQueue.push(ScrollPacket(startSpots, stopSpots, lineNums, msgs));
}

void IOHandler::stopAllScrollingText() {
    std::lock_guard<std::mutex> lock(queueLock);
    scrollQueue.push(ScrollPacket(true));
}

void IOHandler::textScroller()
{
    // these only become as large as however many lines are actually scrolled (not always three)
    std::vector<std::string> toScreen;
    std::vector<size_t> spotInMsgs;
    ScrollPacket packet(true);
    unsigned int lastPrint = 0;
    std::unique_lock<std::mutex> runlock(runLock, std::defer_lock);
    std::unique_lock<std::mutex> queuelock(queueLock, std::defer_lock);
    bool newPacket = false;

    while (true) {

        // check if we should return
        runlock.lock();
        if (run==false) {
            return;
        }

        // check for new packets in queue
        queuelock.lock();
        if (!scrollQueue.empty()) {
            packet = scrollQueue.front();
            scrollQueue.pop();
            queuelock.unlock();
            newPacket = true;
        }
        else {
            queuelock.unlock();
        }
        runlock.unlock();

        // the rest of the loop runs independently of the object

        if (packet.sleep==true) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            continue;
        }

        if (newPacket) {
            newPacket = false;

            // process the new packet
            for (size_t x = 0; x < packet.msgsForLines.size(); x++) {
                packet.msgsForLines.at(x).append(" ");
                packet.msgsForLines.at(x).append(packet.msgsForLines.at(x));
                toScreen.push_back(packet.msgsForLines.at(x).substr(0, packet.stopSpots.at(x) - packet.startSpots.at(x) + 1));
                spotInMsgs.push_back(0);
            }
        }

        // this part scrolls the current packet
        if (millis() - lastPrint > 250) {
            for (size_t i = 0; i < packet.msgsForLines.size(); i++) {
                printToLCD(toScreen.at(i), packet.lineNums.at(i)*20 + packet.startSpots.at(i));
                spotInMsgs.at(i) = (spotInMsgs.at(i) + 1) % (packet.msgsForLines.at(i).size()/2);
                toScreen.at(i) = packet.msgsForLines.at(i).substr(spotInMsgs.at(i), toScreen.at(i).size());
            }
            lastPrint = millis();
        }
    }
}


