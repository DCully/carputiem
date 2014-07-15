#include "Controller.h"
#include <iostream>
#include "wiringPi.h"
#include "../ObdSerial/ObdFactory.h"
#include "../MusicPlayer/MusicScreenFactory.h"

/// successfully exits this ctor (remember to un-comment iohandler stuff)
/// TODO: test MusicManager

Controller::Controller() {

    /*
    * This tries to set up each part of the program, but will continue
    * if one of them fails. The iohandler object, however, uses the
    * wiringPi library, which terminates the program internally,
    * so for testing without the Pi, I comment that line out.
    */

    ///iohandler = new IOHandler(8,9,12,11,10,0,1,2,3,4,5,6,7);

    try{
        // set up OBD stuff
        obd = new ObdSerial("/dev/ttyUSB0");
        ObdFactory obdf(obd);
        std::vector<ScreenData*> pages;
        obdf.buildObdScreens(obd->getSuppdCmds(), pages);
        screenDataManager.addScreens(pages, "obdPages", "home", 2);
    }
    catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << ", continuing" << std::endl;
    }
    try {
        // set up music playing stuff
        musicManager = new MusicManager("/home/david/Music/from linnett/");
        MusicScreenFactory msf;
        msf.buildScreens(*musicManager, screenDataManager);

    }
    catch (std::exception& e) {
        std::cerr << "ERROR: " << e.what() << ", continuing" << std::endl;
    }
    // finish setting up the controller regardless of past success
    lastPush = 1;
    ///screenDataManager.getCurrentScreenData().printPage(*iohandler);

}


Controller::~Controller() {
    delete obd;
    delete musicManager;
    delete iohandler;
}

extern Controller * controller; // created in main.cpp as entry point for program (needed for static interrupt functions)

// millis() wraps every 49 days
// these are static functions because you can't register NSMFs to button interrupts
void Controller::staticLeftButPressed() {
    controller->leftButPressed();
}

void Controller::staticRightButPressed() {
    controller->rightButPressed();
}

void Controller::staticSelectPressed() {
    controller->selectPressed();
}

void Controller::leftButPressed() {
    if (millis() - lastPush > 200) { // smooth inputs
        lastPush = millis();
        screenDataManager.getCurrentScreenData().moveCursorLeft(*iohandler);
    }
}

void Controller::rightButPressed() {
    if (millis() - lastPush > 200) { // smooth inputs
        lastPush = millis();
        screenDataManager.getCurrentScreenData().moveCursorRight(*iohandler);
    }

}

void Controller::selectPressed() {
    if (millis() - lastPush > 200) {
        lastPush = millis();
        screenDataManager.doCurrentSpotSelectBehavior(*iohandler, *this);
    }
}

ScreenData& Controller::getCurPage() {
    return screenDataManager.getCurrentScreenData();
}


void Controller::update(ObserverPacket& obsp) {
    // forward the generic packet to the current page (the pages know what to cast it to)
    getCurPage().getLineSetupBehavior()->updateLine(iohandler, obsp);
}

