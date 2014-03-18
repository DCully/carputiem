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
    //controller = new Controller();
    //test
    //controller->iohandler->update();
    ObdSerial * obdtest = new ObdSerial("/dev/ttyUSB0");
    vector<int> pids;
    for (size_t x = 0; x < 3; x++) {
        pids.push_back(12);
    }
    obdtest->setFocusedPIDs(pids);
    obdtest->start();
    sleep(10);
    obdtest->setRunStatus(false);
    delete obdtest;
    return 0;
}

