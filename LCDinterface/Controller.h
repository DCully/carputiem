#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <vector>
#include "mockIOHandler.h"
#include "IOHandler.h"
#include "../Observable.h"
#include "ScreenData.h"
#include "../ObdSerial/ObdSerial.h"
#include "ScreenDataManager.h"
#include "../MusicPlayer/MusicManager.h"

class IOHandler;

/*
*   At this point, this object basically delegates all actual "controller"
*   duties to the screenDataManager object it owns.
*/
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

        void update(ObserverPacket& obsp);

    private:
        void leftButPressed();
        void rightButPressed();
        void selectPressed();

        IOHandlerInterface* iohandler;
        MusicManager* musicManager;
        ScreenDataManager screenDataManager;
        ObdSerial * obd;

};


#endif // CONTROLLER_H
