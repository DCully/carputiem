#include "./ObdSerial/ObdSerial.h"
#include "./LCDinterface/IOHandler.h"
#include "./LCDinterface/Controller.h"
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <vector>

using namespace std;

Controller * controller;

int main()
{
    controller = new Controller();
    IOHandler* ioh = new IOHandler(8,9,12,11,10,0,1,2,3,4,5,6,7, controller);

    /// 0) do basic stuff with ioh (just testing IOHandler itself)
    ioh->startScrollText(3,15, 2, "long scrolling string! yaaaaaaaay...");
    ioh->printToLCD("hello world!", 0);
    sleep(15);
    ioh->stopScrollTextOnLine(2);
    sleep(3);

    /// 1) make a screendata object and pass it to ioh->printPage (testing lineSetupBehavior object)
    Observable* obs = new Observable();
    PageChangeBehavior p = PageChangeBehavior();
    string t = "Page Title";

    vector<string> lines;
    lines.push_back("text for line 1");
    lines.push_back("long text for line 2, should scroll");
    lines.push_back("line 3");

    LineSetupBehavior ls = LineSetupBehavior(lines, t);

    ScreenData sd = ScreenData(obs, p, ls);

    ioh->printPage(sd);

    sleep(15);

    /// 2) do the same thing with a labeledLineSetupBehavior object (that implementation isn't done yet)
/*
    string t2 = "long: a a a a a a a a a";

    vector<string> labels;
    lines.push_back("kmh");
    lines.push_back("m/s");
    lines.push_back("*C");

    vector<size_t> spaces;
    spaces.push_back(5);
    spaces.push_back(4);
    spaces.push_back(6);

    LabeledLineSetupBehavior ls2 = LabeledLineSetupBehavior(lines, labels, spaces, t);

    ScreenData sd2 = ScreenData(obs, p, ls2);

    ioh->printPage(sd2);
*/
    /// 3) put sd and sd2 into controller->pages, and test buttoning through pages, calling update, etc

    /// 4) write setUpObdScreens, and integration test it with the whole front end of the program

    /// 5) build hardware into box

    /// 6) set up the software to run at startup, hook up the front end to the back end, and test in the car


    // clean up
    delete obs;
    delete ioh;
    delete controller;
    return 0;
}

