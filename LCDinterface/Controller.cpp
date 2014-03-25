#include "Controller.h"
#include <iostream>
#include "wiringPi.h"

using namespace std;

extern Controller * controller; //this is a reference to the controller object that is created by thread main

Controller::Controller() {
    iohandler = new IOHandler(8,9,12,11,10,0,1,2,3,4,5,6,7, this);
    lastPush = 1;
    ObdSerial obd = ObdSerial("/dev/ttyUSB0");
    obd.start();
    setUpObdScreens(obd);

    /// choose an initial page?

}
Controller::~Controller() {
}

/// strategy pattern

void Controller::setUpObdScreens(ObdSerial& obd) {

    vector<int> pids = obd.getSuppdCmds();
    vector<string> textForLines;
    vector<string> labelsForLines;
    string title = "OBD Data";

    for (size_t page = 0; page < pids.size()/3 + 1; page++) {

        for (size_t pid = 0; (pid < 3) && (3*page + pid<pids.size()); pid++) {
            textForLines.at(pid) = obdcmds_mode1[pids.at(3*page + pid)].human_name;
            labelsForLines.at(pid) = obdcmds_mode1[pids.at(3*page + pid)].units;
        }
        //ScreenData pg = ScreenData(&obd, title, textForLines, labelsForLines);

       // pages.push_back(pg);

    }
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
    // unhook page from observed
    controller->curPage.doLeavePageBehavior();
    controller->curPage.observed->removeObserver(controller->iohandler);

    // swap in the next page (from the left)
    controller->pages.push_back(controller->curPage);
    controller->curPage = controller->pages.front();
    controller->pages.pop_front();

    // hook up new page to observed
    controller->curPage.doLoadPageBehavior();
    controller->curPage.observed->registerObserver(controller->iohandler);

    // print the new page
    controller->iohandler->printPage(controller->curPage);
}

void Controller::changePageRight(void) {
    // unhook page from observed
    controller->curPage.doLeavePageBehavior();
    controller->curPage.observed->removeObserver(controller->iohandler);

    // swap in the next page (from the right)
    controller->pages.push_front(controller->curPage);
    controller->curPage = controller->pages.back();
    controller->pages.pop_back();

    // hook up new page to observed
    controller->curPage.doLoadPageBehavior();
    controller->curPage.observed->registerObserver(controller->iohandler);

    // print the new page
    controller->iohandler->printPage(controller->curPage);
}
