#include "./ObdSerial/ObdSerial.h"
#include "./LCDinterface/IOHandler.h"
#include "./LCDinterface/Controller.h"
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <vector>

Controller * controller;
/*
TODO:

1) TEST: LSB, LLSB, and update() calls

--- get it working up to here, then continue ---

1) write setUpObdScreens and test

2) make cursor invisible and place a special character on the cursored spot

3) build hardware layer and get it working in the car (after presentation)

*/

using namespace std;

int main()
{
    // front end testing
    controller = new Controller();
    sleep(20);

    string data = "data";
    for (int x = 0; x < 9; x++) {
        controller->iohandler->update((x%3)+1, data);
        usleep(250000);
    }
    // back end testing
    /*
    ObdSerial* obd = new ObdSerial("/dev/ttyUSB0");
    obd->setFocusedPIDs(obd->getSuppdCmds);
    obd->start();
    sleep(20);
    delete obd;
    */

    return 0;
}

