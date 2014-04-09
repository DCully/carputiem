#include "./ObdSerial/ObdSerial.h"
#include "./LCDinterface/IOHandler.h"
#include "./LCDinterface/Controller.h"
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <vector>

Controller * controller;

using namespace std;

int main()
{
    controller = new Controller();
   // IOHandler* ioh = new IOHandler(8,9,12,11,10,0,1,2,3,4,5,6,7, controller);


    /// 1) make this screendata object, set it as curPage, and print
 /*
    Observable* obs = new Observable();
    PageChangeBehavior p = PageChangeBehavior();
    string t = "Page Title";

    vector<string> lines;
    lines.push_back("scrolling text for line 1");
    lines.push_back("long text for line 2 should scroll as well");
    lines.push_back("static line 3");

    LineSetupBehavior ls = LineSetupBehavior(lines, t);

    ScreenData sd = ScreenData(obs, p, ls);
*/
    /// 2) do the same thing with this labeledLineSetupBehavior object
/*
    string t2 = "long: a a a a a a a a a";

    vector<string> labels;
    labels.push_back("kmh");
    labels.push_back("m/s");
    labels.push_back("*C");

    vector<size_t> spaces;
    spaces.push_back(5);
    spaces.push_back(4);
    spaces.push_back(6);

    LabeledLineSetupBehavior ls2 = LabeledLineSetupBehavior(lines, labels, spaces, t);

    ScreenData sd2 = ScreenData(obs, p, ls2);

    ioh->printPage(sd2);
*/
    /// 3) put sd and sd2 into controller->pages, and test:
            /// moving cursor with L and R buttons,
            /// changing pages with S button,
            /// calling update once
            /// calling update repeatedly while text is scrolling

    /// 4) write setUpObdScreens, and integration test it with the whole front end of the program

    sleep(20);

    return 0;
}

