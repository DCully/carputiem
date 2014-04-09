#include "Controller.h"
#include <iostream>
#include "wiringPi.h"

// change ScreenData cursorSpots to vector  - rewriting button interrupt functions

/// TODO: fix everything...

extern Controller * controller; //this is a reference to the controller object that is created in main.cpp


using namespace std;



Controller::Controller() {

    //obd = new ObdSerial("/dev/ttyUSB0");

    //int  pidCount = obd->getSuppdCmds().size();

  //  obdPages = new ScreenData[pidCount/3 + 1];
 //   obdlsbs = new LineSetupBehavior[pidCount/3 + 1];
  //  obdpcbs = new PageChangeBehavior[pidCount/3 + 1];

//    setUpObdScreens(obd->getSuppdCmds()); // fills pages deque

/// TESTING - all this needs to go outside of ctor

    obs = new Observable();
    p = PageChangeBehavior();
    string t = "Page Title";

    vector<string> lines;
    lines.push_back("scrolling text for line 1");
    lines.push_back("long text for line 2 should scroll as well");
    lines.push_back("static line 3");

    ls = LineSetupBehavior(lines, t);

    sd = ScreenData(obs, p, ls);
    cout << "builds first test page ok" << endl;

    string t2 = "long: a a a a a a a a a";

    vector<string> labels;
    labels.push_back("kmh");
    labels.push_back("m/s");
    labels.push_back("*C");

    vector<size_t> spaces;
    spaces.push_back(5);
    spaces.push_back(4);
    spaces.push_back(6);

    ls2 = LabeledLineSetupBehavior(lines, labels, spaces, t);
    cout << "builds second linesetupbehavior ok" << endl;
    sd2 = ScreenData(obs, p, ls2);
    cout << "builds second test page ok" << endl;
    curPageIndex = 0;
    pages.push_back(sd);
    pages.push_back(sd2);

    cout << "adds two test pages OK" << endl;

    cout << pages.at(0).getCurrentCursorSpot() << endl;

    cout << curPageIndex << endl;

    iohandler = new IOHandler(8,9,12,11,10,0,1,2,3,4,5,6,7, this);
    lastPush = 1;
    cout << this << endl;

    iohandler->printPage(pages.at(curPageIndex));
}

Controller::~Controller() {
    delete iohandler;
    delete obs;

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
    cout << "in getcurpage" << endl;
    cout << "curPageIndex is: " << curPageIndex << endl;
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










