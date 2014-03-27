#include "LineSetupBehaviors.h"
#include <iostream>
#include "IOHandler.h"

using namespace std;

/// for base class (one static or scrolled string per line)

LineSetupBehavior::LineSetupBehavior(std::vector<std::string> txtForLines) : textForLines(txtForLines)
{
    updateSpotsForLines.push_back(0);
    updateSpotsForLines.push_back(0);
    updateSpotsForLines.push_back(0);
}

LineSetupBehavior::LineSetupBehavior() {

}

void LineSetupBehavior::renderLine(IOHandler* iohandler, size_t lineNum) {
    if (lineNum != 0 && lineNum != 1 && lineNum != 2) {
        cerr << "Invalid line number passed to LineSetupBehavior::renderLine" << endl;
        return ;
    }
    if (textForLines.at(lineNum).size() > 20) {
        iohandler->startScrollText(0, 19, lineNum, textForLines.at(lineNum));
    }
    else {
        iohandler->printToLCD(textForLines.at(lineNum), 20 + 20*lineNum);
    }
}

void LineSetupBehavior::updateLine(IOHandler* iohandler, size_t lineNum, string info) {
    if (lineNum != 0 || lineNum != 1 || lineNum != 2) {
        cerr << "Invalid line number passed to LineSetupBehavior::updateLine" << endl;
        return ;
    }
    textForLines.at(lineNum) = info;
    renderLine(iohandler, lineNum);
}

/// ----------------------------------------------------------------------------

/// for derived class LabeledLineSetupBehavior (labels on right side of screen)

LabeledLineSetupBehavior::LabeledLineSetupBehavior(std::vector<std::string> textForLines,
    std::vector<std::string> lblsForLines, std::vector<size_t> spaceBtwnLblsAndTxtOnLines)
    : LineSetupBehavior(textForLines), labelsForLines(lblsForLines), spaceBtwnLblsAndTextOnLines(spaceBtwnLblsAndTxtOnLines)
{
    for (size_t line = 0; line < 3; line++) {

        // determine update spot (left justified)
        updateSpotsForLines.push_back(20 - labelsForLines.at(line).size() - 1);

        // determine end of scrolling section (for textForLines)
        endOfScrollForLines.push_back(20 - labelsForLines.at(line).size() - spaceBtwnLblsAndTextOnLines.at(line));

    }
}

void LabeledLineSetupBehavior::renderLine(IOHandler* iohandler, size_t lineNum) {

    if (lineNum != 0 && lineNum != 1 && lineNum != 2) {
        cerr << "Invalid line number passed to LabeledLineSetupBehavior::renderLine" << endl;
        return ;
    }

    // prints with blanks for data fields

    if (textForLines.at(lineNum).size() <= endOfScrollForLines.at(lineNum)) {

        string output = textForLines.at(lineNum); // "blahblah"

        for (size_t x = 0; x < 20 - labelsForLines.at(lineNum).size(); x++) {
            output.append(" ");
        } // "blahblah          "

        output.append(labelsForLines.at(lineNum));
        /// assert: should always be 20 chars long

        iohandler->printToLCD(output, 20 + 20*lineNum);
    }
    else {

        iohandler->startScrollText(0, endOfScrollForLines.at(lineNum), lineNum, textForLines.at(lineNum));

        string output = "";

        while (output.size() < spaceBtwnLblsAndTextOnLines.at(lineNum)) {
            output.append(" ");
        }

        output.append(labelsForLines.at(lineNum));

        /// assert: output.size() = 20-endOfScrollForLines.at(lineNum)

        iohandler->printToLCD(output, endOfScrollForLines.at(lineNum) + 1);

    }
}

void LabeledLineSetupBehavior::updateLine(IOHandler* iohandler, size_t lineNum, string info) {
    /// print info to updateSpotForLine(lineNum) - deal with update spots being right justified
    iohandler->printToLCD(info, 20 + 20*lineNum + updateSpotsForLines.at(lineNum) - info.size());

}



/// -----------------------------------------------------------------------------

/// other derived classes go down here