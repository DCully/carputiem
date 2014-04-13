#include "./ObdSerial/ObdSerial.h"
#include "./LCDinterface/IOHandler.h"
#include "./LCDinterface/Controller.h"
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <vector>

Controller * controller;

/// implement a stopscrolltextonline method in iohandler... static prints from lsbs arent cancelling old scroll threads

/// how to get a smoother looking interface with the scrolling prints, using the cursor for manual input and output?
/// - place a certain symbol wherever the current cursor position is? and then make the actual cursor's movements invisible?

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

