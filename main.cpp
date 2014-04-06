#include "./ObdSerial/ObdSerial.h"
#include "./LCDinterface/IOHandler.h"
#include "./LCDinterface/Controller.h"
#include <iostream>
#include <sstream>
#include <unistd.h>

using namespace std;

Controller * controller;

int main()
{
    controller = new Controller();
    IOHandler* ioh = new IOHandler(8,9,12,11,10,0,1,2,3,4,5,6,7, controller);

    // do stuff with ioh
    std::string msg = "long scrolling string!";
    ioh->startScrollText(0,12, 2, msg);
    std::string msg2 = "hello sir";
    ioh->printToLCD(msg2, 0);
    sleep(45);
    ioh->stopScrollTextOnLine(2);
    ioh->stopScrollTextOnLine(3);
    sleep(3);
    delete controller;
    return 0;
}

