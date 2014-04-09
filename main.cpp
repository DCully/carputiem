#include "./ObdSerial/ObdSerial.h"
#include "./LCDinterface/IOHandler.h"
#include "./LCDinterface/Controller.h"
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <vector>

Controller * controller;

/// Problems:

/// 1) the LLSB renderline code is terrible and doesnt work

/// 2) need to make ScreenData objects own their LSB pointers and PCB pointers



/// I also need to:
   /// - test calling update, and with scrolling text running
   /// - write setUpObdScreens, and test with a dummy vector
   /// - test pageChangeBehavior stuff


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

