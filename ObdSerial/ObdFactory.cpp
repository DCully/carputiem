#include "ObdFactory.h"
#include "obdcomms.h"
#include "../LCDinterface/ScreenData.h"
#include "../LCDinterface/LineSetupBehaviors.h"
#include "../LCDinterface/PageChangeBehaviors.h"
#include <vector>

using std::vector;
using std::string;

ObdFactory::ObdFactory(ObdSerial* obs) {
    obds = obs;
}

ObdFactory::~ObdFactory() {
    // this object doesn't own it's observable object - it just builds ScreenData objects for it
}

// builds ScreenData objects for each grouping of three PIDs passed in, using obdcmds_mode1
void ObdFactory::buildObdScreens(const std::vector<int>& obdIndices, std::vector<ScreenData>& pages) {

    for (size_t page = 0; page < obdIndices.size()/3 + 1; page++) {

        /// build an OPCB with next (up to) 3 PIDs from obdIndices
        vector<int> pids;
        vector<string> textForLines;
        vector<string> labelsForLines;
        vector<size_t> spacesForData;

        for (size_t index = page*3; index < page*3 + 3 && index < obdIndices.size(); index++) {

            pids.push_back(obdIndices.at(index));

            /// build the three other vectors for the LLSB

        }
        ObdPageChangeBehavior* opcb = new ObdPageChangeBehavior(pids, obds);

        /// build an LLSB
        string title = "OBD Data ";
        title.append(std::to_string(page));
        LabeledLineSetupBehavior* llsb = new LabeledLineSetupBehavior(textForLines, labelsForLines, spacesForData, title);

        /// push a new ScreenData back into pages
        ScreenData sd = ScreenData(obds, opcb, llsb);
        pages.push_back(sd);
    }

}
