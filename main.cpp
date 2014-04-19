#include "./ObdSerial/ObdSerial.h"
#include "./LCDinterface/IOHandler.h"
#include "./LCDinterface/Controller.h"
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <vector>

extern Controller * controller;

using namespace std;

int main()
{
    // front end testing
    controller = new Controller();
    sleep(20);
/*
    string data = "123";
    for (int x = 0; x < 9; x++) {
        controller->iohandler->update((x%3)+1, data);
        usleep(250000);
    }
    // back end testing

    ObdSerial* obd = new ObdSerial("/dev/ttyUSB0");
    obd->setFocusedPIDs(obd->getSuppdCmds);
    obd->start();
    sleep(20);
    delete obd;
    */

    return 0;
}

