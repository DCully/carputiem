#ifndef SCREENDATA_H
#define SCREENDATA_H
#include "../Observable.h"
#include <deque>
#include <string>

typedef void (*SelectBehaviorFunc)(void);

/// this class offers a page title and three lines of text, which scroll if longer than 20 characters
/// the (potentially) scrolling text can be updated in its entirety by a model object calling "update"
/// dataUpdateSpots are where IOHandler will print what it gets sent via update() (within each line)
class ScreenData {

    friend class Controller;

    public:
        ScreenData(Observable * obs, std::string t, std::vector<std::string> lTxt);
        ScreenData();
        ~ScreenData();

        // for wherever we end up building these things from
        void addCursorSpot(std::pair<int, SelectBehaviorFunc> newSpot);

        // for iohandler
        std::string getTitle();
        std::string getTextForLine(size_t num);
        bool islabeled;
        bool updatesAreLeftJustified;
        /// etc.

    protected:
        // for controller
        Observable * observed;                            // the address of the object this is for
        std::deque< std::pair<int, SelectBehaviorFunc> > cursorSpots;   // places on this screen the cursor can go to, with corresponding select behaviors
        std::pair<int, SelectBehaviorFunc> currentSpot;

        // for IOHandler
        std::string title; // title of page
        std::string scrolledLineText[3]; // these are the ones considered for scrollability in IOHandler
};

/// This class enhances the basic scrolling line functionality by adding labels and data slots.
/// Here, the scrolledLineText vector from ScreenData scrolls from spot 0-X on each line
/// for each line (number n), X = 20 - lineLbls[n].size() - maxLengthOfDataOnLine[n]
/// calling IOHandler's update() on these pages updates the data, not the scrolling text

class LabeledScreenData: public ScreenData {
    public:
        LabeledScreenData(Observable * obs, std::string t, std::vector<std::string> scrLinTxt, std::vector<std::string> lLbls, std::vector<int> lDataMaxLengths);
    private:
        // for IOHandler
        std::string lineLbls[3];
        int maxLengthOfDataFieldOnLine[3];
};

#endif //SCREENDATA_H
