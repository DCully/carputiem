#include "ObdSerial.h"
#include "IOHandler.h"
#include "Controller.h"
using namespace std;

Controller * controller; //this is the global controller object created by thread main in order handle interrupts

int main()
{
    //controller = new Controller();
    //test
    //controller->iohandler->update();
    ObdSerial * obdtest = new ObdSerial("/dev/ttyUSB0");
    //obdtest->start();
    //obdtest->start();
    delete obdtest;
    return 0;
}

