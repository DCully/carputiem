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

1) TEST: allocated number of spaces for data field in LLSB is off by one
    - entire LSB/LLSB source is rewritten - see if problem persists

2) TEST: LSB, LLSB, and update() calls

3) TEST: every few page switches, it ends up printing the same page again, or starting to print a new page and then registering two interrupts
    - what causes this? threw print statements everywhere to help figure it out

--- get it working up to here, then continue ---

4) test update() calls with scrolling text running, etc

5) write setUpObdScreens

6) test setUpObdScreens with a dummy vector, and get the whole front end to demonstration level

BY NEXT THURSDAY:

1) make cursor invisible and place a special character on the cursored spot

2) build hardware layer and get it working in the car (after presentation)

*/

using namespace std;

int main()
{
    // front end testing
    controller = new Controller();
    sleep(20);

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

