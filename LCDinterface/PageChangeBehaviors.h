#ifndef PAGECHANGEBEHAVIORS_H
#define PAGECHANGEBEHAVIORS_H

#include "../Observable.h"
#include "../ObdSerial/ObdSerial.h"

// this family of classes encapsulates page changing behavior for all screendata objects
// passed to ScreenData objects on their construction
class PageChangeBehavior {
    public:
        virtual void loadPage(Observable& obs);
        virtual void leavePage(Observable& obs);
};

class ObdPageChangeBehavior: public PageChangeBehavior {
    public:
        ObdPageChangeBehavior(std::vector<int>& pidIndices, ObdSerial* obd);
        void loadPage(ObdSerial& obs);
        void leavePage(ObdSerial& obs);
    private:
        std::vector<int> PIDsIShowFromSuppdCmds;
        ObdSerial * obd;
};

#endif
