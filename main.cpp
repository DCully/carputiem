#include "./ObdSerial/ObdSerial.h"
#include "./LCDinterface/IOHandler.h"
#include "./LCDinterface/Controller.h"
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <vector>

Controller * controller;

/// there's some issue in LLSB::renderLine where it doesnt stop scrolling threads and puts scrolling threads down over title lines
/// make sure there can only ever be 3 scrolling threads going at once... too many threads bogs the Pi down
/// does each scrolling line need its own thread? or could all scrolling lines be handled by a single separate thread...

/// Test:
   /// - LLSBs
   /// - PCBs
   /// - update(), with scrolling text running
   /// - write setUpObdScreens (factory), and test with a dummy vector


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

