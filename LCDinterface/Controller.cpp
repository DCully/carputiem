#include "Controller.h"
#include <iostream>
#include "wiringPi.h"

// change ScreenData cursorSpots to vector  - rewriting button interrupt functions

using namespace std;

extern Controller * controller; //this is a reference to the controller object that is created by thread main

Controller::Controller() {
   // iohandler = new IOHandler(8,9,12,11,10,0,1,2,3,4,5,6,7, this);
 //  lastPush = 1;
   // obd = new ObdSerial("/dev/ttyUSB0");

  //  int  pidCount = obd->getSuppdCmds().size();

  //  obdPages = new ScreenData[pidCount/3 + 1];
  //  obdlsbs = new LineSetupBehavior[pidCount/3 + 1];
  //  obdpcbs = new PageChangeBehavior[pidCount/3 + 1];

  //  setUpObdScreens(obd->getSuppdCmds()); // fills pages deque

//    curPage = 0;
/*
    curPage = pages.front();
    pages.pop_front();
*/
    //iohandler->printPage(curPage);

}

Controller::~Controller() {


    /// what else needs to be deleted in here??

    //delete obd;
  //  delete iohandler;
   // delete [] obdlsbs;
   // delete [] obdpcbs;
   // delete [] obdPages;

}


void Controller::setUpObdScreens(vector<int> pids) {
/*
    string title = "OBD Data";

    for (size_t page = 0; page < pids.size()/3 + 1; page++) {

        vector<int> pidIndices;
        vector<string> lineText;
        vector<string> lineLbl;
        vector<size_t> spaceBtwnLblsAndText;

        for (size_t pid = 0; (pid < 3) && (3*page + pid<pids.size()); pid++) {

            // this sets up the vectors to build the ScreenData objects

            pidIndices.push_back(pids.at(3*page+pid));
            lineText.push_back(obdcmds_mode1[pids.at(3*page+pid)].human_name);
            lineLbl.push_back(obdcmds_mode1[pids.at(3*page+pid)].*units);

            string max_val = obdcmds_mode1[pids.at(3*page+pid)].max_value;

            spaceBtwnLblsAndText.push_back(obdcmds_mode1[pids.at(3*page+pid)].max_val.size());

        }

        // build LabeledLineSetupBehavior and ObdPageChangeBehavior for each page

        obdlsbs[page] =
        obdpcbs[page] =
        obdPages[page] =

    }
    */
}

// millis() wraps every 49 days
// these are static functions because you can't register NSMFs to button interrupts
// they contain their logic here because left and right buttons always do the same thing on every page
void Controller::lButPressed() {
    if (millis() - controller->lastPush > 100) { // smooth inputs
        controller->lastPush = millis();

        // get next cursor position from curPage and tell iohandler to move to there
//        controller->getCurPage()->cursorSpots.push_back(controller->getCurPage()->currentSpot);
  //      controller->getCurPage()->currentSpot = controller->getCurPage()->cursorSpots.front();
  //      controller->getCurPage()->cursorSpots.pop_front();
   //     controller->iohandler->moveCursor(controller->getCurPage()->currentSpot.first);
    }
}
void Controller::rButPressed() {
    if (millis() - controller->lastPush > 100) { // smooth inputs
        controller->lastPush = millis();

        // get next cursor position from curPage and tell iohandler to move to there
 //       controller->getCurPage()->cursorSpots.push_front(controller->getCurPage()->currentSpot);
 //       controller->getCurPage()->currentSpot = controller->getCurPage()->cursorSpots.back();
 //       controller->getCurPage()->cursorSpots.pop_back();
 //       controller->iohandler->moveCursor(controller->getCurPage()->currentSpot.first);
    }
}

void Controller::selPressed() {
    if (millis() - controller->lastPush > 100) {
        controller->lastPush = millis();
        // call curPage for behavior at currentSpot
 //       controller->getCurPage()->cursorSpots.at(currentSpot).second();
    }
}

ScreenData* Controller::getCurPage() {
    return &pages.at(curPage);
}

void Controller::changePageLeft(void) {
    // unhook page from observed
    controller->pages.at(controller->curPage).doLeavePageBehavior();
    controller->pages.at(controller->curPage).observed->removeObserver(controller->iohandler);

    // shift to the next page (from the left)
//    curPage = (controller->curPage+1)%pages.size();

    // hook up new page to observed
    controller->getCurPage()->doLoadPageBehavior();
    controller->getCurPage()->observed->registerObserver(controller->iohandler);

    // print the new page
//    controller->iohandler->printPage(controller->getCurPage());
}

void Controller::changePageRight(void) {
    // unhook page from observed
//    controller->pages.at(curPage).doLeavePageBehavior();
 //   controller->pages.at(curPage).observed->removeObserver(controller->iohandler);

    // swap in the next page (from the right)
 //   curPage = (curPage+1)%pages.size();

    // hook up new page to observed
 //   controller->pages.at(curPage).doLoadPageBehavior();
 //   controller->pages.at(curPage).observed->registerObserver(controller->iohandler);

    // print the new page
//    controller->iohandler->printPage(controller->getCurPage());
}
