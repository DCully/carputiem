#include "Controller.h"
#include <iostream>
#include "wiringPi.h"

extern Controller * controller; //this is a reference to the controller object that is created in main.cpp

using namespace std;

Controller::Controller() {
/*
    obd = new ObdSerial("/dev/ttyUSB0");

    int  pidCount = obd->getSuppdCmds().size();

    obdPages = new ScreenData[pidCount/3 + 1];
    obdlsbs = new LineSetupBehavior[pidCount/3 + 1];
    obdpcbs = new PageChangeBehavior[pidCount/3 + 1];

    setUpObdScreens(obd->getSuppdCmds()); // fills pages deque
*/

    /// TESTING
    PageChangeBehavior* p;
    LineSetupBehavior* ls;
    LabeledLineSetupBehavior* ls2;

    obs = new Observable();
    p = new PageChangeBehavior();
    PageChangeBehavior* p2 = new PageChangeBehavior();
    string t = "Page Title";

    vector<string> lines;
    lines.push_back("Vehicle Identification Number");
    lines.push_back("AH1S57F3246E12JD");
    lines.push_back("2006 Honda Civic");

    vector<string> lines2;
    lines2.push_back("Current Vehicle Speed Over Ground");
    lines2.push_back("Other Datum");
    lines2.push_back("Temperature");


    ls = new LineSetupBehavior(lines, t);

    string t2 = "Diff. Title";

    vector<string> labels;
    labels.push_back("kmh");
    labels.push_back("m/s");
    labels.push_back("*C");

    vector<size_t> spaces;
    spaces.push_back(5);
    spaces.push_back(4);
    spaces.push_back(6);

    ls2 = new LabeledLineSetupBehavior(lines2,  labels, spaces, t2);

    curPageIndex = 0;

    pages.push_back(ScreenData(obs, p, ls));


    pages.push_back(ScreenData(obs, p2, ls2));


    iohandler = new IOHandler(8,9,12,11,10,0,1,2,3,4,5,6,7, this);
    lastPush = 1;
    iohandler->printPage(pages.at(curPageIndex));
}

Controller::~Controller() {
    delete iohandler;
    delete obs; // should be obd, eventually

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










