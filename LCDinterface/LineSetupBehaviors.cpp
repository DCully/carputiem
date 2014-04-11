#include "LineSetupBehaviors.h"
#include <iostream>
#include "IOHandler.h"

using namespace std;

/// for base class (one static or scrolled string per line)

LineSetupBehavior::LineSetupBehavior(vector<string> txtForLines, const string& t) : textForLines(txtForLines)
{
    title = t.substr(0,15);
    while (title.size() < 17) {
        title.append(" ");
    }
    title = title + "<->";

    updateSpotsForLines.push_back(0);
    updateSpotsForLines.push_back(0);
    updateSpotsForLines.push_back(0);
    updateSpotsForLines.push_back(0);
}

LineSetupBehavior::LineSetupBehavior() {

}

void LineSetupBehavior::renderLine(IOHandler* iohandler, size_t lineNum) {

    cout << "linesetupbehavior's renderline called for " << lineNum << endl;

    if (lineNum != 0 && lineNum != 1 && lineNum != 2 && lineNum != 3) {
        cerr << "Invalid line number passed to LineSetupBehavior::renderLine" << endl;
        return ;
    }
    if (lineNum == 0) {
        iohandler->printToLCD(title, 0);
        return ;
    }

    if (textForLines.at(lineNum-1).size() > 20) {
        iohandler->startScrollText(0, 19, lineNum, textForLines.at(lineNum-1));
    }
    else {
        iohandler->printToLCD("                    ", 20*lineNum);
        iohandler->printToLCD(textForLines.at(lineNum-1), 20*lineNum);
    }
}

// reprints entire line
void LineSetupBehavior::updateLine(IOHandler* iohandler, size_t lineNum, string info) {

    if (lineNum != 1 && lineNum != 2 && lineNum != 3) {
        cerr << "Invalid line number passed to LineSetupBehavior::updateLine" << endl;
    }
    else {
        textForLines.at(lineNum) = info;
        renderLine(iohandler, lineNum);
    }

}




/// for derived class LabeledLineSetupBehavior (labels on right side of screen)

LabeledLineSetupBehavior::LabeledLineSetupBehavior() {}

LabeledLineSetupBehavior::LabeledLineSetupBehavior(
    std::vector<std::string> txtForLines,
    std::vector<std::string> lblsForLines,
    std::vector<size_t> sForDataOnLs,
    const std::string& t) :
    labelsForLines(lblsForLines),
    spacesForDataOnLine(sForDataOnLs)
{

    // makes title contain entire top line's text
    textForLines = txtForLines;

    title = t.substr(0,15);
    while (title.size() < 17) {
        title.append(" ");
    }
    title = title + "<->";

    /// determine end of scroll for lines for lines 1-3
    for (size_t line = 0; line < 3; line++) {
        size_t spot = 19 - lblsForLines.at(line).size() - sForDataOnLs.at(line);
        endOfScrollsForLine.push_back(spot); // storing at indexes 0,1,2... shift-corrected to 1,2,3 in renderLine
        updateSpotsForLines.push_back(spot + sForDataOnLs.at(line));
    }

}

void LabeledLineSetupBehavior::renderLine(IOHandler* iohandler, size_t lineNum) {

    cout << "labeledlinesetupbehavior's renderline called for " << lineNum << endl;

    if (lineNum != 0 && lineNum != 1 && lineNum != 2 && lineNum != 3) {
        cerr << "Invalid line number passed to LabeledLineSetupBehavior::renderLine" << endl;
        return ;
    }

    // printing title line - working
    if (lineNum == 0) {
        iohandler->printToLCD(title, 0);
        return ;
    }

    /// shifting lineNum (no data stored for line 0 == title line
    lineNum--;

    /// print with blanks for data fields, lines 1-3
    size_t spaceForText = 20 - labelsForLines.at(lineNum).size() - spacesForDataOnLine.at(lineNum);

    if (textForLines.at(lineNum).size() <=  spaceForText ) {

        /// static label on the line

        string output = textForLines.at(lineNum);

        // adding spaces for text
        while (output.size() < spaceForText) {
            output.append(" ");
        }

        // adding spaces for data from update call
        for (size_t x = 0; x < spacesForDataOnLine.at(lineNum); x++) {
            output.append(" ");
        }

        output.append(labelsForLines.at(lineNum));

        if (output.size() != 20) {
            cerr << "Error in LabeledLineSetupBehavior::renderLine - output.size() != 20 for line " << lineNum << endl;
        }

        iohandler->printToLCD(output, 20 + 20*lineNum);

    }
    else {
        /// scrolling label on line
        iohandler->startScrollText(0, spaceForText, lineNum, textForLines.at(lineNum));
        string output = "";
        while (output.size() < spacesForDataOnLine.at(lineNum)) {
            output.append(" ");
        }
        output.append(labelsForLines.at(lineNum));

        iohandler->printToLCD(output, 20 + 20*lineNum + spaceForText + 1);
    }

}

/// not currently being tested (4/9)
void LabeledLineSetupBehavior::updateLine(IOHandler* iohandler, size_t lineNum, string info) {

    if (lineNum != 1 && lineNum != 2 && lineNum != 3) {
        cerr << "Invalid line number passed to LabeledLineSetupBehavior::updateLine" << endl;
        return ;
    }

    // prints info to updateSpotForLine(lineNum), and deals with update spots being right justified
    iohandler->printToLCD(info, 20 + 20*lineNum + updateSpotsForLines.at(lineNum) - info.size());

}






