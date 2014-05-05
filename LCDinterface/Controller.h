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

        /*
         * ISO-C++ forbids non-static member function pointers... so I did this
         * to route calls from interrupt calls to member functions through the global pointer.
         * this is definitely not very well designed. It does work, however.
        */
        static void staticLeftButPressed();
        static void staticRightButPressed();
        static void staticSelectPressed();
        static void staticChangePageRight();
        static void staticChangePageLeft();

        ScreenData& getCurPage();
        unsigned int lastPush;

    private:
        void leftButPressed();
        void rightButPressed();
        void selectPressed();
        void changePageLeft();
        void changePageRight();

        IOHandler* iohandler;

        std::vector<ScreenData> pages;
        int curPageIndex;
        ObdSerial * obd;

};


#endif // CONTROLLER_H
