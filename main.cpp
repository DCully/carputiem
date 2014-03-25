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
    controller = new Controller();
    sleep(20);
    //delete obdtest;
    return 0;
}

