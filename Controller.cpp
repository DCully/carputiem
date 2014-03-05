#include "Controller.h"
#include <iostream>
#include "wiringPi.h"

using namespace std;

extern Controller * controller; //this is a reference to the controller object that is created by thread main

/// build a factory to add ObdSerial object ScreenData page structs into "pages"

Controller::Controller() {
    iohandler = new IOHandler(8,9,12,11,10,0,1,2,3,4,5,6,7, this);
    iohandler->update();
    lastPush = 1;
}
Controller::~Controller() {
}
// millis() wraps every 49 days
// remember: these are static functions, because we can't register NSMFs to button interrupts
void Controller::lButPressed() {
    if (millis() - controller->lastPush > 100) { // smooth inputs
        //cout << "left button pressed at " << controller->getCurPos() << endl;
        controller->lastPush = millis();
        // get next cursor position from curPage and tell iohandler to move to there
        int x = controller->getCurPageInfo()->curSpots.front();
        controller->getCurPageInfo()->curSpots.pop_front();
        controller->getCurPageInfo()->curSpots.push_back(x);
        controller->iohandler->moveCursor(x);
    }
}
void Controller::rButPressed() {
    if (millis() - controller->lastPush > 100) { // smooth inputs
        //cout << "right button pressed at " << controller->getCurPos() << endl;
        controller->lastPush = millis();
        // get next cursor position from curPage and tell iohandler to move to there
        int x = controller->getCurPageInfo()->curSpots.back();
        controller->getCurPageInfo()->curSpots.pop_back();
        controller->getCurPageInfo()->curSpots.push_front(x);
        controller->iohandler->moveCursor(x);
    }
}
void Controller::selPressed() {
    if (millis() - controller->lastPush > 100) {
        //cout << "select pressed at " << controller->getCurPos() << endl;
        if (controller->iohandler->getCurPos() == 19) { // next page right
            controller->changePage(false);
        }
        else if (controller->iohandler->getCurPos() == 17) { // next page left
            controller->changePage(true);
        }
        controller->lastPush = millis();
    }
}
ScreenData* Controller::getCurPageInfo() {
    return &curPage;
}
void Controller::changePage(bool left) {
    // unregisters iohandler as observer, changes curPage, re-registers as observer, and updates
    curPage.observed->removeObserver(iohandler);
    if (left) {
        // logic to go left in deque
        pages.push_back(curPage);
        curPage = pages.front();
        pages.pop_front();
    }
    else {
        // logic to go right in deque
        pages.push_front(curPage);
        curPage = pages.back();
        pages.pop_back();
    }
    curPage.observed->registerObserver(iohandler);
    iohandler->update();
}
