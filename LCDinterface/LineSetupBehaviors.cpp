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

LabeledLineSetupBehavior::LabeledLineSetupBehavior(std::vector<std::string> txtForLines,
    std::vector<std::string> lblsForLines, std::vector<size_t> spaceBtwnLblsAndTxtOnLines, const string& t)
    : labelsForLines(lblsForLines),
      spaceBtwnLblsAndTextOnLines(spaceBtwnLblsAndTxtOnLines)
{
    textForLines = txtForLines;

    title = t.substr(0,15);
    while (title.size() < 17) {
        title.append(" ");
    }
    title = title + "<->";

    updateSpotsForLines.push_back(0);
    endOfScrollForLines.push_back(0);

    // for lines 1-3
    for (size_t line = 0; line < 3; line++) {

        // determine update spot (left justified)
        updateSpotsForLines.push_back(20 - labelsForLines.at(line).size() - 1);

        // determine end of scrolling section (for textForLines)
        endOfScrollForLines.push_back(20 - labelsForLines.at(line).size() - spaceBtwnLblsAndTextOnLines.at(line));

    }
    cout << "in labeledlinesetupbehavior constructor: " << endl;
    cout << textForLines.at(0) << endl;
    cout << labelsForLines.at(0) << endl;
    cout << title << endl;

}

void LabeledLineSetupBehavior::renderLine(IOHandler* iohandler, size_t lineNum) {

    cout << "labeledlinesetupbehavior's renderline called for " << lineNum << endl;

    if (lineNum != 0 && lineNum != 1 && lineNum != 2 && lineNum != 3) {
        cerr << "Invalid line number passed to LabeledLineSetupBehavior::renderLine" << endl;
        return ;
    }

    // printing title line
    if (lineNum == 0) {
        iohandler->printToLCD(title, 0);
        return ;
    }

    // prints with blanks for data fields, lines 1-3

    if (textForLines.at(lineNum).size() <= endOfScrollForLines.at(lineNum)) {
        string output = textForLines.at(lineNum); //here it's "blahblah"
        for (size_t x = 0; x < 20 - labelsForLines.at(lineNum).size(); x++) {
            output.append(" ");
        } // now it's "blahblah          "

        output.append(labelsForLines.at(lineNum));
        /// assert: should always be 20 chars long

        iohandler->printToLCD(output, 20*lineNum);
    }
    else {

        iohandler->startScrollText(0, endOfScrollForLines.at(lineNum), lineNum, textForLines.at(lineNum));

        string output = "";

        // add requisite number of spaces
        while (output.size() < spaceBtwnLblsAndTextOnLines.at(lineNum)) {
            output.append(" ");
        }

        // add the label
        output.append(labelsForLines.at(lineNum));

        /// assert: output.size() = 20-endOfScrollForLines.at(lineNum)

        iohandler->printToLCD(output, 20 * lineNum + endOfScrollForLines.at(lineNum) + 1);

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






