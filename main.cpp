#include "./ObdSerial/ObdSerial.h"
#include "./LCDinterface/IOHandler.h"
#include "./LCDinterface/Controller.h"
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <vector>

Controller * controller;

/// printPage for labeledlinesetupbehavior looks like its calling linesetupbehavior's renderline functions



using namespace std;

int main()
{
    controller = new Controller();

    sleep(20);

    return 0;
}

