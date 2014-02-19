#include "Controller.h"
#include <iostream>
#include "wiringPi.h"
extern Controller * controller; //this is a reference to the controller object that is created by thread main
using namespace std;
Controller::Controller() {
    setViewPage(1);
    iohandler = new IOHandler(8,9,12,11,10,0,1,2,3,4,5,6,7, this);
    iohandler->update();
    lastPush = 1;
}
//button interrupts smoothed - only run every 1/5th of a second, max
//millis() wraps every 49 days - no car runs for that long
void Controller::lButPressed() {
    if (millis() - controller->lastPush > 200) {
        cout << "left button pressed at " << controller->getCurPos() << endl;
        controller->lastPush = millis();
    }
}
void Controller::rButPressed() {
    if (millis() - controller->lastPush > 200) {
        cout << "right button pressed at " << controller->getCurPos() << endl;
        controller->lastPush = millis();
    }
}
void Controller::selPressed() {
    if (millis() - controller->lastPush > 200) {
        cout << "select pressed at " << controller->getCurPos() << endl;
        controller->lastPush = millis();
    }
}
int Controller::getCurPos() {
   return iohandler -> getCurPos();
}
ScreenData* Controller::getInfoAddresses() {
    ScreenData * sd = new ScreenData;
    sd->title = "Title";
    sd->pageNum = viewPage;
    sd->dataArray[0] = 11;
    sd->dataArray[1] = 12.02;
    sd->dataArray[2] = 1234.1234;
    sd->nameArray[0] = "dave";
    sd->nameArray[1] = "bobalsoincharge";
    sd->nameArray[2] = "charlesincharge";
    sd->lblArray[0] = "mp";
    sd->lblArray[1] = "sphh";
    sd->lblArray[2] = "mph";
    return sd;
}
void Controller::setViewPage(unsigned int input) {
    viewPage = input;
}
