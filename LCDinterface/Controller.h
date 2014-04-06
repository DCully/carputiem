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

    friend class ScreenData;
    friend class IOHandlerTest;

    public:
        Controller();
        ~Controller();

        /*These three functions handle button input calls from
        the iohandler, routing them as necessary.
        */
        static void lButPressed();
        static void rButPressed();
        static void selPressed();

        ScreenData* getCurPage();
        unsigned int lastPush;
    private:
        // called in constructor, to build ObdSerial's ScreenData objects
        void setUpObdScreens(std::vector<int> pids);
        ScreenData* obdPages;
        LineSetupBehavior* obdlsbs;
        PageChangeBehavior* obdpcbs;
        

        static void changePageLeft(void);
        static void changePageRight(void);
        IOHandler * iohandler;
        std::vector<ScreenData> pages;
        int curPage;
        ObdSerial * obd;
};


#endif // CONTROLLER_H
