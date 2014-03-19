#include "Controller.h"
#include <iostream>
#include "wiringPi.h"

using namespace std;

extern Controller * controller; //this is a reference to the controller object that is created by thread main

/// TODO:
/// 1) make a factory to produce ScreenData objects for ObdSerial et. al.

Controller::Controller() {
    iohandler = new IOHandler(8,9,12,11,10,0,1,2,3,4,5,6,7, this);
    lastPush = 1;
}
Controller::~Controller() {
}
// millis() wraps every 49 days
// these are static functions because you can't register NSMFs to button interrupts
// they contain their logic here because left and right buttons always do the same thing on every page
void Controller::lButPressed() {
    if (millis() - controller->lastPush > 100) { // smooth inputs
        controller->lastPush = millis();

        // get next cursor position from curPage and tell iohandler to move to there
        controller->getCurPage()->cursorSpots.push_back(controller->getCurPage()->currentSpot);
        controller->getCurPage()->currentSpot = controller->getCurPage()->cursorSpots.front();
        controller->getCurPage()->cursorSpots.pop_front();
        controller->iohandler->moveCursor(controller->getCurPage()->currentSpot.first);
    }
}
void Controller::rButPressed() {
    if (millis() - controller->lastPush > 100) { // smooth inputs
        controller->lastPush = millis();

        // get next cursor position from curPage and tell iohandler to move to there
        controller->getCurPage()->cursorSpots.push_front(controller->getCurPage()->currentSpot);
        controller->getCurPage()->currentSpot = controller->getCurPage()->cursorSpots.back();
        controller->getCurPage()->cursorSpots.pop_back();
        controller->iohandler->moveCursor(controller->getCurPage()->currentSpot.first);
    }
}

void Controller::selPressed() {
    if (millis() - controller->lastPush > 100) {
        controller->lastPush = millis();
        // call curPage for behavior at currentSpot
        controller->getCurPage()->currentSpot.second();
    }
}

ScreenData* Controller::getCurPage() {
    return &curPage;
}

void Controller::changePageLeft(void) {
    // unregisters iohandler as observer, changes curPage, re-registers as observer, and updates
    controller->curPage.observed->removeObserver(controller->iohandler);
    controller->pages.push_back(controller->curPage);
    controller->curPage = controller->pages.front();
    controller->pages.pop_front();
    controller->curPage.observed->registerObserver(controller->iohandler);
    controller->iohandler->printPage(controller->curPage);
}

void Controller::changePageRight(void) {
    // unregisters iohandler as observer, changes curPage, re-registers as observer, and updates
    controller->curPage.observed->removeObserver(controller->iohandler);
    controller->pages.push_front(controller->curPage);
    controller->curPage = controller->pages.back();
    controller->pages.pop_back();
    controller->curPage.observed->registerObserver(controller->iohandler);
    controller->iohandler->printPage(controller->curPage);
}
