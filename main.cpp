#include "./ObdSerial/ObdSerial.h"
#include "./LCDinterface/IOHandler.h"
#include "./LCDinterface/Controller.h"
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <vector>

Controller * controller;

/// 1) Test:
   /// - LLSBs
   /// - update(), with scrolling text running
   /// - write setUpObdScreens (factory), and test with a dummy vector

/// 2) build hardware layer and get it working in the car

/// 3) make cursor invisible and place a special character at the "cursored spot" on the screen

/// 4) do other stuff like that to make the project better

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

