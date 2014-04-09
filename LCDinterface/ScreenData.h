#ifndef SCREENDATA_H
#define SCREENDATA_H

#include "IOHandler.h"
#include "../Observable.h"
#include <vector>
#include <string>
#include "PageChangeBehaviors.h"
#include "LineSetupBehaviors.h"

/// note: heavy use of PIMPL

/// make this class own its lineSetupBehavior and pageChangeBehavior objects?

typedef void (*SelectBehaviorFunc)(void);

class LineSetupBehavior;

class ScreenData {

    public:
        ScreenData(Observable* obs, PageChangeBehavior pcb, LineSetupBehavior* lsb);
        ScreenData();
        ~ScreenData();

        void moveCursorLeft(IOHandler* ioh);
        void moveCursorRight(IOHandler* ioh);
        void doCurSpotSelectBehavior();
        void addCursorSpot(std::pair<int, SelectBehaviorFunc> newSpot);
        void doLoadPageBehavior();
        void doLeavePageBehavior();
        const int getCurrentCursorSpot() const;
        LineSetupBehavior* getLineSetupBehavior();
        Observable* observed;
    private:
        // set in constructor
        PageChangeBehavior pageChangeBehavior;
        LineSetupBehavior* lineSetupBehavior;

        // places on this screen the cursor can go to, with corresponding select behaviors
        std::vector< std::pair<int, SelectBehaviorFunc> > cursorSpots;
        int currentSpotIndex;
};


#endif //SCREENDATA_H
