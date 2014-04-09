#include "./ObdSerial/ObdSerial.h"
#include "./LCDinterface/IOHandler.h"
#include "./LCDinterface/Controller.h"
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <vector>

Controller * controller;

/// Problems:

/// 1) printPage for labeledlinesetupbehavior looks like its calling
/// linesetupbehavior's renderline functions, despite them being virtual in
/// that (base) class... how to fix? what's going wrong?

/// 2) cursor printing movement after page changes looks terrible, but I think
/// it is actually working as intended. How to make it look better? Place an X at
/// the cursor'd spot, and turn the blink off? ...



/// I also need to:
   /// - test calling update, and with scrolling text running
   /// - write setUpObdScreens, and test with a dummy vector
   /// - test pageChangeBehavior stuff


using namespace std;

int main()
{
    controller = new Controller();

    sleep(20);

    return 0;
}

