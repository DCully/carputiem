#include "ObdFactory.h"
#include "obdcomms.h"
#include "../LCDinterface/ScreenData.h"
#include "../LCDinterface/LineSetupBehaviors.h"
#include "../LCDinterface/PageChangeBehaviors.h"
#include <vector>
#include <iostream>

ObdFactory::ObdFactory(ObdSerial* obs) {
    obds = obs;
}

ObdFactory::~ObdFactory() {
    // this object doesn't own it's observable object - it just builds ScreenData objects for it
}

// builds ScreenData objects for each grouping of three PIDs passed in, using obdcmds_mode1
void ObdFactory::buildObdScreens(const std::vector<int>& obdIndices, std::vector<ScreenData*>& pages) {

    for (size_t page = 0; page < (obdIndices.size()+2)/3; page++) {

        /// build an OPCB with next (up to) 3 PIDs from obdIndices
        std::vector<int> pids;
        std::vector<std::string> textForLines;
        std::vector<std::string> labelsForLines;
        std::vector<size_t> spacesForData;

        for (size_t index = page*3; index < page*3 + 3 && index < obdIndices.size(); index++) {

            pids.push_back(obdIndices.at(index));

            /// build the three other vectors for the LLSB

            textForLines.push_back(obdcmds_mode1[obdIndices.at(index)].human_name);
            labelsForLines.push_back(obdcmds_mode1[obdIndices.at(index)].units);

            // better way to handle nonzero trailing digits?
            std::string maxval = std::to_string( (int) obdcmds_mode1[obdIndices.at(index)].max_value);
            std::string minval = std::to_string( (int) obdcmds_mode1[obdIndices.at(index)].min_value);


            size_t maxlen = (maxval.size() > minval.size() ? maxval.size() : minval.size());

            spacesForData.push_back(maxlen);
        }
        ObdPageChangeBehavior* opcb = new ObdPageChangeBehavior(pids);

        /// build an LLSB
        std::string title = "OBD Data ";
        title.append(std::to_string(page));
        LabeledLineSetupBehavior* llsb = new LabeledLineSetupBehavior(textForLines, labelsForLines, spacesForData, title);

        /// push a new ScreenData back into pages
        ScreenData* sd = new ScreenData(obds, opcb, llsb);
        pages.push_back(sd);
    }

}
/*
void ObdFactory::buildVINScreen(const std::string& VIN, std::vector<ScreenData>& pages) {

    PageChangeBehavior* pcb = new PageChangeBehavior();

    std::vector<std::string> lines;
    lines.push_back("Vehicle Identification Number");
    lines.push_back(VIN);
    lines.push_back(" ");

    LineSetupBehavior* lsb = new LineSetupBehavior(lines, "VIN Screen");

    pages.push_back(ScreenData(obds, pcb,lsb));
}

*/




