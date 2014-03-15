#include "./ObdSerial/ObdSerial.h"
#include "./LCDinterface/IOHandler.h"
#include "./LCDinterface/Controller.h"

#include <iostream>
#include <sstream>
using namespace std;

Controller * controller; //this is the global controller object created by thread main in order handle interrupts

int main()
{
    //controller = new Controller();
    //test
    //controller->iohandler->update();
    //ObdSerial * obdtest = new ObdSerial("/dev/ttyUSB0");
    //obdtest->start();
    //cout << "VIN read from main is: " << obdtest->getVIN() << endl;
    //obdtest->start();

    string hex = "43";
    stringstream ss;
    ss << std::hex << hex;
    int x;
    ss >> x;
    cout << (char) x << endl;



    //delete obdtest;
    return 0;
}

