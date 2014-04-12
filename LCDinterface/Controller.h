#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <vector>
#include "IOHandler.h"
#include "../Observable.h"
#include "ScreenData.h"
#include "../ObdSerial/ObdSerial.h"

class IOHandler;

class Controller
{

    public:
        Controller();
        ~Controller();

        /*These three functions handle button input calls from
        the iohandler, routing them as necessary.
        */

        // ISO-C++ forbids non-static member function pointers... so I did this
        static void lButPressed();
        static void rButPressed();
        static void selPressed();
        static void staticChangePageRight();
        static void staticChangePageLeft();

        ScreenData* getCurPage();
        unsigned int lastPush;
    private:
        void changePageLeft(void);
        void changePageRight(void);
        // called in constructor, to build ObdSerial's ScreenData objects
        void setUpObdScreens(std::vector<int> pids);
        ScreenData* obdPages;
        LineSetupBehavior* obdlsbs;
        PageChangeBehavior* obdpcbs;

        IOHandler * iohandler;
        std::vector<ScreenData> pages;
        int curPageIndex;
        ObdSerial * obd;

        /// TESTING
        Observable* obs;

};


#endif // CONTROLLER_H
