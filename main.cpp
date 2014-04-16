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

0) get update calls working for LSB

1) write setUpObdScreens and test it

2) make cursor invisible and place a special character on the cursored spot (memento pattern?)

3) build hardware layer and get it working in the car (after presentation)

*/

using namespace std;

int main()
{
    // front end testing
    controller = new Controller();
    sleep(20);

    string data = "123";
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

