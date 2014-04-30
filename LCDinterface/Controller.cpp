#include "Controller.h"
#include <iostream>
#include "wiringPi.h"
#include "../ObdSerial/ObdFactory.h"


Controller * controller; //this is a reference to the controller object that is created in main.cpp

using namespace std;

Controller::Controller() {

    obd = new ObdSerial("/dev/ttyUSB0");

    iohandler = new IOHandler(8,9,12,11,10,0,1,2,3,4,5,6,7, this);

    ObdFactory obdfact = ObdFactory(obd); // builds the obdfactory we will use to fill "pages"
    obdfact.buildObdScreens(obd->getSuppdCmds(), pages); // builds obd data screens into "pages"
    obdfact.buildVINScreen(obd->getVIN(), pages); // adds in the VIN screen
    /// add in DTCs screen

    curPageIndex = 0;
    lastPush = 1;
    pages.at(curPageIndex).printPage(iohandler);
}

Controller::~Controller() {
    delete iohandler;
    delete obd; // should be obd, eventually

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

void Controller::staticChangePageLeft() {
    controller->changePageLeft();
}

void Controller::changePageLeft() {
    // unhook page from observed
    pages.at(curPageIndex).doLeavePageBehavior();
    pages.at(curPageIndex).observed->removeObserver(iohandler);

    // shift to the next page (from the left)
    curPageIndex = (curPageIndex+1)%pages.size();

    // hook up new page to observed
    pages.at(curPageIndex).doLoadPageBehavior();
    pages.at(curPageIndex).observed->registerObserver(iohandler);

    // print the new page
    pages.at(curPageIndex).printPage(iohandler);
}

void Controller::staticChangePageRight() {
    controller->changePageRight();
}

void Controller::changePageRight() {

    // unhook page from observed
    pages.at(curPageIndex).doLeavePageBehavior();
    pages.at(curPageIndex).observed->removeObserver(iohandler);

    // shift to the next page (from the right)
    curPageIndex = (curPageIndex-1)%pages.size();

    // hook up new page to observed
    pages.at(curPageIndex).doLoadPageBehavior();
    pages.at(curPageIndex).observed->registerObserver(iohandler);

    // print the new page
    pages.at(curPageIndex).printPage(iohandler);

    // get cursor to right arrow (it starts at the left arrow by default)
    pages.at(curPageIndex).moveCursorRight(iohandler);
}

