#ifndef SCREENDATA_H
#define SCREENDATA_H
#include "Observable.h"
#include <deque>
struct ScreenData {
    std::string title;                  // title for page, displayed on line one in both display modes
    std::string nameArray [3];          // for ObdSerial pages AND other pages
    std::string lblArray [3];           // for ObdSerial pages
    double dataArray [3];               // for ObdSerial pages
    bool dataPage;                      // true if this represents a page for ObdSerial
    Observable * observed;              // the address of the object this is for
    std::deque<int> curSpots;           // places on this screen the cursor can go to
};

#endif //SCREENDATA_H
