
#include "PageChangeBehaviors.h"

using namespace std;

/// these objects run the interaction between the controller objects (screendata) and model objects

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


