#ifndef PAGECHANGEBEHAVIORS_H
#define PAGECHANGEBEHAVIORS_H

#include "../Observable.h"
#include "../ObdSerial/ObdSerial.h"
#include "../MusicPlayer/MusicManager.h"

// this family of classes encapsulates page changing behavior for all screendata objects
// gets passed to ScreenData objects on their construction
class PageChangeBehavior {
    public:
        virtual ~PageChangeBehavior() {}
        virtual void loadPage(Observable& obs);
        virtual void leavePage(Observable& obs);
        virtual PageChangeBehavior* clone() const { return new PageChangeBehavior(*this); }

};

class ObdPageChangeBehavior: public PageChangeBehavior {
    public:
        virtual ~ObdPageChangeBehavior() {}
        ObdPageChangeBehavior(std::vector<int>& pidIndices);
        void loadPage(ObdSerial& obs);
        void leavePage(ObdSerial& obs);
        ObdPageChangeBehavior* clone() const { return new ObdPageChangeBehavior(*this); } // shallow copy on obd is ok
    private:
        std::vector<int> PIDsIShowFromSuppdCmds;
};

#endif
