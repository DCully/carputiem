
#include "PageChangeBehaviors.h"

using namespace std;

void PageChangeBehavior::loadPage(Observable& obs) {
    // default behavior is to do nothing
}

void PageChangeBehavior::leavePage(Observable& obs) {
    // default behavior is to do nothing
}

ObdPageChangeBehavior::ObdPageChangeBehavior(vector<int>& pidIndices, ObdSerial * obds) {

    for (size_t x = 0; x < 3 && x < pidIndices.size(); x++) {
        PIDsIShowFromSuppdCmds.push_back(pidIndices.at(x));
    }

    obd = obds;
}

void ObdPageChangeBehavior::loadPage(ObdSerial& obs) {
    obd->setFocusedPIDs(PIDsIShowFromSuppdCmds);
}

void ObdPageChangeBehavior::leavePage(ObdSerial& obs) {

}


