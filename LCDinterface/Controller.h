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
* This is the Controller object for the program - although at this point,
* it delegates a lot of functionality to the ScreenDataManager.
* This class owns all model objects - the MusicManager and ObdSerial.
* It also owns the ScreenDataManager, and recieves updates from model objects,
* which it routes to the current ScreenData manager for processing.
*/
class Controller : public Observer
{

    public:
        Controller();
        ~Controller();

        /*
         * ISO-C++ forbids non-static member function pointers... so I made static functions,
         * to route calls from interrupts to member functions through the global pointer.
         * this definitely does not feel not very well designed. It does work, though.
        */

        /*
        * Function to catch the left button's interrupt.
        */
        static void staticLeftButPressed();

        /*
        * Function to catch the right button's interrupt.
        */
        static void staticRightButPressed();

        /*
        * Function to catch the select button's interrupt.
        */
        static void staticSelectPressed();

        /*
        * for smoothing inputs
        */
        unsigned int lastPush;

        /*
        * This is the function that model objects (Observables) call.
        * The Controller object routes it to the current ScreenData's LineSetupBehavior.
        */
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
