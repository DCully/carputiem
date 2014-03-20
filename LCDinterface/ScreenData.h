#ifndef SCREENDATA_H
#define SCREENDATA_H
#include "../Observable.h"
#include <deque>
#include <string>

typedef void (*SelectBehaviorFunc)(void);

class ScreenData {

    friend class Controller;
    friend class IOHandler;

    public:
        // there are two types of ScreenData objects:

        // 1) construct with simple string vector sized 3 - these will be your three lines
        //      - this type of ScreenData object rewrites an entire line when updating
        ScreenData(Observable * obs, std::string t, std::vector<std::string> lTxt);

        // 2) construct the object with a vector for text, and also a label for a data field
        //      - these will scroll the scrLinText on the left side,
        //        leaving space on the right for an updateable datum (1 per line)
        ScreenData(Observable * obs, std::string t, std::vector<std::string> scrLinTxt, std::vector<std::string> lLbls);

        ScreenData();
        ~ScreenData();

        void addCursorSpot(std::pair<int, SelectBehaviorFunc> newSpot);

        // for iohandler
        std::string getTitle();
        std::string getTextForLine(size_t num);
        bool haslabels;


    protected:
        // for controller
        Observable * observed;                            // the address of the object this is for
        std::deque< std::pair<int, SelectBehaviorFunc> > cursorSpots;   // places on this screen the cursor can go to, with corresponding select behaviors
        std::pair<int, SelectBehaviorFunc> currentSpot;

        // for IOHandler
        std::string title; // title of page
        std::string scrolledLineText[3]; // these are the ones considered for scrolling in IOHandler
        std::string lineLbls[3]; // labels of data parameters
};

#endif //SCREENDATA_H
