#include "Controller.h"
#include <iostream>
#include "wiringPi.h"
#include "../ObdSerial/ObdFactory.h"


Controller * controller; //this is a reference to the controller object that is created in main.cpp
using namespace std;

Controller::Controller() {
/*
    obd = new ObdSerial("/dev/ttyUSB0");

*/
    std::vector<int> pids;
    pids.push_back(4); // load
    pids.push_back(5); // coolant temp
    pids.push_back(12); // rpm
    pids.push_back(17); // throttle position
    pids.push_back(31); // time since start
    pids.push_back(51); // barometric pressure
    pids.push_back(14);
    pids.push_back(5);

    ObdFactory obdfact = ObdFactory(obd);
    obdfact.buildObdScreens(pids, pages);

    curPageIndex = 0;
    lastPush = 1;
    iohandler = new IOHandler(8,9,12,11,10,0,1,2,3,4,5,6,7, this);

    pages.at(curPageIndex).printPage(iohandler);
/// NOTE: OPCB ISR routines commented out to cut off ObdSerial object
///       some bits of changePageLeft/Right are also commented out to avoid ObdSerial references

}

Controller::~Controller() {
    delete iohandler;
    ///delete obd;

}

// millis() wraps every 49 days
// these are static functions because you can't register NSMFs to button interrupts
void Controller::lButPressed() {

    if (millis() - controller->lastPush > 200) { // smooth inputs
        controller->lastPush = millis();
        controller->getCurPage()->moveCursorLeft(controller->iohandler);

    }

}
void Controller::rButPressed() {

    if (millis() - controller->lastPush > 200) { // smooth inputs
        controller->lastPush = millis();
        controller->getCurPage()->moveCursorRight(controller->iohandler);

    }

}

void Controller::selPressed() {
    if (millis() - controller->lastPush > 200) {
        controller->lastPush = millis();
        controller->getCurPage()->doCurSpotSelectBehavior();
    }
}

ScreenData* Controller::getCurPage() {

    return &pages.at(curPageIndex);
}

void Controller::staticChangePageLeft(void) {
    controller->changePageLeft();
}

void Controller::changePageLeft(void) {
    // unhook page from observed
    pages.at(curPageIndex).doLeavePageBehavior();
    ///pages.at(curPageIndex).observed->removeObserver(iohandler);

    // shift to the next page (from the left)
    curPageIndex = ( curPageIndex == 0 ? pages.size() - 1 : (curPageIndex-1)%pages.size() );

    // hook up new page to observed
    pages.at(curPageIndex).doLoadPageBehavior();
    ///pages.at(curPageIndex).observed->registerObserver(iohandler);

    // print the new page
    pages.at(curPageIndex).printPage(iohandler);
}

void Controller::staticChangePageRight(void) {
    controller->changePageRight();
}

void Controller::changePageRight(void) {

    // unhook page from observed
    pages.at(curPageIndex).doLeavePageBehavior();
    ///pages.at(curPageIndex).observed->removeObserver(iohandler);

    // shift to the next page (from the right)
    curPageIndex = (curPageIndex+1)%pages.size();


    // hook up new page to observed
    pages.at(curPageIndex).doLoadPageBehavior();
    ///pages.at(curPageIndex).observed->registerObserver(iohandler);

    // print the new page
    pages.at(curPageIndex).printPage(iohandler);
}











