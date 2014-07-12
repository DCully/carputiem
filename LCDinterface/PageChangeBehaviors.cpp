
#include "PageChangeBehaviors.h"

/// these objects run the interaction between the controller objects (screendata) and model objects

void PageChangeBehavior::loadPage(Observable& obs) {
    // default behavior is to do nothing
}

void PageChangeBehavior::leavePage(Observable& obs) {
    // default behavior is to do nothing
}

ObdPageChangeBehavior::ObdPageChangeBehavior(std::vector<int>& pidIndices) {

    for (size_t x = 0; x < 3 && x < pidIndices.size(); x++) {
        PIDsIShowFromSuppdCmds.push_back(pidIndices.at(x));
    }
}

void ObdPageChangeBehavior::loadPage(ObdSerial& obs) {
    obs.setFocusedPIDs(PIDsIShowFromSuppdCmds);
}

void ObdPageChangeBehavior::leavePage(ObdSerial& obs) {

}




