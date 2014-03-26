#ifndef SCREENDATA_H
#define SCREENDATA_H

#include "../Observable.h"
#include <deque>
#include <string>
#include "PageChangeBehaviors.h"
#include "LineSetupBehaviors.h"

typedef void (*SelectBehaviorFunc)(void);

class LineSetupBehavior;

class ScreenData {

    friend class Controller;

    public:
        ScreenData(Observable* obs, PageChangeBehavior pcb, LineSetupBehavior lsb, std::string titl);
        ScreenData();
        ~ScreenData();

        void addCursorSpot(std::pair<int, SelectBehaviorFunc> newSpot);
        void doLoadPageBehavior();
        void doLeavePageBehavior();
        LineSetupBehavior* getLineSetupBehavior();
    protected:
        // set in constructor
        PageChangeBehavior pageChangeBehavior;
        LineSetupBehavior lineSetupBehavior;
        Observable* observed;

        // places on this screen the cursor can go to, with corresponding select behaviors
        std::deque< std::pair<int, SelectBehaviorFunc> > cursorSpots;
        std::pair<int, SelectBehaviorFunc> currentSpot;

        std::string title;
};


#endif //SCREENDATA_H
