#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <vector>
#include "IOHandler.h"
#include "../Observable.h"
#include "ScreenData.h"
#include "../ObdSerial/ObdSerial.h"
#include "ScreenDataManager.h"

class IOHandler;

class Controller : public Observer
{

    public:
        Controller();
        ~Controller();

        /*
         * ISO-C++ forbids non-static member function pointers... so I did this, in order
         * to route calls from interrupts to member functions through the global pointer.
         * this definitely does not feel not very well designed. It does work, though.
        */
        static void staticLeftButPressed();
        static void staticRightButPressed();
        static void staticSelectPressed();

        ScreenData& getCurPage();
        unsigned int lastPush;

        void update(size_t linenum, std::string infoToPrint);

    private:
        void leftButPressed();
        void rightButPressed();
        void selectPressed();

        IOHandler* iohandler;

        ScreenDataManager screenDataManager;
        ObdSerial * obd;

};


#endif // CONTROLLER_H
