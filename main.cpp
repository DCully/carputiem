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

    return 0;
}

