#ifndef SCREENDATA_H
#define SCREENDATA_H
#include "../Observable.h"
#include <deque>
#include <string>

typedef void (*SelectBehaviorFunc)(void);

class ScreenData {
    public:
        ScreenData(Observable * obs, std::string t);      // add 17 and 19 to curSpots in constructor
        ScreenData();
        ~ScreenData();
        void addCursorSpotToLeft(std::pair<int, SelectBehaviorFunc>);
        void addCursorSpotToRight(std::pair<int, SelectBehaviorFunc>);

        std::vector<int> updateLinePositions; // spots on each line to print data at when it's received via an update() call
        std::string title;                                // title for page
        Observable * observed;                            // the address of the object this is for
        std::deque< std::pair<int, SelectBehaviorFunc> > cursorSpots;   // places on this screen the cursor can go to, with corresponding select behaviors
        std::pair<int, SelectBehaviorFunc> currentSpot;
};


#endif //SCREENDATA_H
