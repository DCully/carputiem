#include "Controller.h"
#include <iostream>

extern Controller * controller; //this is a reference to the controller object that is created by thread main

using namespace std;

Controller::Controller() {
    setViewPage(1);
    iohandler = new IOHandler(8,9,12,11,10,0,1,2,3,4,5,6,7, this);
}
void Controller::lButPressed() {
    cout << "left button pressed at " << controller->getCurPos() << endl;
}
void Controller::rButPressed() {
    cout << "right button pressed at " << controller->getCurPos() << endl;
}
void Controller::selPressed() {
 cout << "select pressed at " << controller->getCurPos() << endl;
}
int Controller::getCurPos() {
   return iohandler -> getCurPos();
}

ScreenData* Controller::getInfoAddresses() {
    ScreenData * sd = new ScreenData;
    sd->title = "hello!!!";
    sd->pageNum = viewPage;
    sd->dataArray[0] = 11;
    sd->dataArray[1] = 12.02;
    sd->dataArray[2] = 1234.1234;
    sd->nameArray[0] = "dave";
    sd->nameArray[1] = "bob";
    sd->nameArray[2] = "charlesthealmighty";
    sd->lblArray[0] = "m/n";
    sd->lblArray[1] = "s/mmmmmmmmm";
    sd->lblArray[2] = "mph";
    return sd;
}

void Controller::setViewPage(unsigned int input) {
    viewPage = input;
}
