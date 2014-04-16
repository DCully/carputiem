#ifndef SCREENDATA_H
#define SCREENDATA_H

#include "IOHandler.h"
#include "../Observable.h"
#include <vector>
#include <string>
#include "PageChangeBehaviors.h"
#include "LineSetupBehaviors.h"

/// changing interface so that controller calls this in order to print the page

typedef void (*SelectBehaviorFunc)(void);

class LineSetupBehavior;

class ScreenData {

    public:
        ScreenData(Observable* obs, PageChangeBehavior* pcb, LineSetupBehavior* lsb);
        ScreenData();
        ~ScreenData();

        ScreenData(const ScreenData& other);
        ScreenData& operator=(ScreenData other);
        void swap(ScreenData& other);

        void moveCursorLeft(IOHandler* ioh);
        void moveCursorRight(IOHandler* ioh);
        void doCurSpotSelectBehavior();

        void doLoadPageBehavior();
        void doLeavePageBehavior();

        void addCursorSpot(std::pair<int, SelectBehaviorFunc> newSpot);

        /// if we separate user's "cursor" from actual cursor, this might not be necessary...?
        const int getCurrentCursorSpot() const;

        LineSetupBehavior* getLineSetupBehavior();

        /// NEW - need to implement
        void printPage(IOHandler* ioh);

        Observable* observed;
    private:
        // set in constructor
        PageChangeBehavior* pageChangeBehavior;
        LineSetupBehavior* lineSetupBehavior;

        // places on this screen the cursor can go to, with corresponding select behaviors
        std::vector< std::pair<int, SelectBehaviorFunc> > cursorSpots;
        int currentSpotIndex;
};


#endif //SCREENDATA_H
