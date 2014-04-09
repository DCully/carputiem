#include "Controller.h"
#include <iostream>
#include "wiringPi.h"

// change ScreenData cursorSpots to vector  - rewriting button interrupt functions

/// TODO: fix everything...

using namespace std;

extern Controller * controller; //this is a reference to the controller object that is created in main.cpp

Controller::Controller() {
    iohandler = new IOHandler(8,9,12,11,10,0,1,2,3,4,5,6,7, this);
    lastPush = 1;
    //obd = new ObdSerial("/dev/ttyUSB0");

    //int  pidCount = obd->getSuppdCmds().size();

  //  obdPages = new ScreenData[pidCount/3 + 1];
 //   obdlsbs = new LineSetupBehavior[pidCount/3 + 1];
  //  obdpcbs = new PageChangeBehavior[pidCount/3 + 1];

//    setUpObdScreens(obd->getSuppdCmds()); // fills pages deque

}

Controller::~Controller() {
   // delete iohandler;
}

// millis() wraps every 49 days
// these are static functions because you can't register NSMFs to button interrupts
void Controller::lButPressed() {

    if (millis() - controller->lastPush > 100) { // smooth inputs
        controller->lastPush = millis();
        controller->getCurPage()->moveCursorLeft(controller->iohandler);

    }

}
void Controller::rButPressed() {

    if (millis() - controller->lastPush > 100) { // smooth inputs
        controller->lastPush = millis();
        controller->getCurPage()->moveCursorRight(controller->iohandler);

    }

}

void Controller::selPressed() {
    if (millis() - controller->lastPush > 100) {
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
    pages.at(curPageIndex).observed->removeObserver(iohandler);

    // shift to the next page (from the left)
    curPageIndex = (curPageIndex+1)%pages.size();

    // hook up new page to observed
    pages.at(curPageIndex).doLoadPageBehavior();
    pages.at(curPageIndex).observed->registerObserver(iohandler);

    // print the new page
    iohandler->printPage(pages.at(curPageIndex));
}

void Controller::staticChangePageRight(void) {
    controller->changePageRight();
}

void Controller::changePageRight(void) {
    // unhook page from observed
    pages.at(curPageIndex).doLeavePageBehavior();
    pages.at(curPageIndex).observed->removeObserver(iohandler);

    // shift to the next page (from the right)
    curPageIndex = (curPageIndex-1)%pages.size();

    // hook up new page to observed
    pages.at(curPageIndex).doLoadPageBehavior();
    pages.at(curPageIndex).observed->registerObserver(iohandler);

    // print the new page
    iohandler->printPage(pages.at(curPageIndex));
}

/// ------------------------------------------

void Controller::setUpObdScreens(vector<int> pids) {

}










