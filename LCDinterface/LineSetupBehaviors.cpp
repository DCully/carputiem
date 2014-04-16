#include "LineSetupBehaviors.h"
#include <iostream>
#include "IOHandler.h"

using namespace std;

/// for base class (one static or scrolled string per line)

LineSetupBehavior::LineSetupBehavior(vector<string> textForLines, const string& t)
{
    titleLine = t.substr(0,15);
    while (titleLine.size() < 17) {
        titleLine.append(" ");
    }
    titleLine = titleLine + "<->";

    for (int x = 1; x < 4; x++) { // loop through textForLines and pull out scrolling ones
        if (textForLines.at(x-1).size() > 20) {
            scrollingLineNums.push_back(x);
            textForScrollingLines.push_back(textForLines.at(x-1));
        }
        else {
            staticLineNums.push_back(x);
            while (textForLines.at(x-1).size() < 20) { // saves time printing later (writes over everything)
                textForLines.at(x-1).append(" ");
            }
            textForStaticLines.push_back(textForLines.at(x-1));
        }
    }
}

LineSetupBehavior::LineSetupBehavior() {

}

void LineSetupBehavior::renderPage(IOHandler* iohandler) {

    iohandler->stopAllScrollingText();
    iohandler->printToLCD(titleLine, 0);

    // print the static lines
    for (size_t x = 0; x < textForStaticLines.size(); x++) {
        iohandler->printToLCD(textForStaticLines.at(x), staticLineNums.at(x)*20);
    }

    // start the scrolling thread for dynamic lines (they all take up the whole line, no need to clear)
    if (textForScrollingLines.size() > 0) {

        vector<size_t> ss;
        vector<size_t> es;

        for (size_t i = 0; i < textForScrollingLines.size(); i++) {
            ss.push_back(0);
            es.push_back(19);
        }

        iohandler->startScrollText(ss, es, scrollingLineNums, textForScrollingLines);
    }
}

void LineSetupBehavior::updateLine(IOHandler* iohandler, size_t lineNum, string info) {

    if (lineNum != 1 && lineNum != 2 && lineNum != 3) {
        cerr << "Invalid line number passed to LineSetupBehavior::updateLine" << endl;
        return ;
    }

    // find where this lineNum is in the two line text arrays and update that spot, then reprint page
    for (size_t x = 0; x < scrollingLineNums.size(); x++) {
        if (scrollingLineNums.at(x) == lineNum) {
            textForScrollingLines.at(x) = info;
            return ;
        }
    }

    for (size_t y = 0; y < staticLineNums.size(); y++) {
        if (staticLineNums.at(y) == lineNum) {
            textForStaticLines.at(y) = info;
            return ;
        }
        else {
            cerr << "Error in LSB::updateLine - lineNum " << lineNum << " not found in either lineNums vector" << endl;
            return ;
        }
    }

    renderPage(iohandler);

}


/// ------------------------------------------------------------------

/// for derived class LabeledLineSetupBehavior (labels on right side of screen)

LabeledLineSetupBehavior::LabeledLineSetupBehavior() {}

LabeledLineSetupBehavior::LabeledLineSetupBehavior(
    std::vector<std::string> textForLs,
    std::vector<std::string> labelsForLs,
    std::vector<size_t> spaceForDataOnLs,
    const std::string& t)
{

    spaceForDataOnLines = spaceForDataOnLs;

    // set up title line
    titleLine = t.substr(0,15);
    while (titleLine.size() < 17) {
        titleLine.append(" ");
    }
    titleLine.append("<->");

    // determine body lines setup
    for (size_t line = 0; line < 3; line++) {

        size_t spaceForText = 20 - labelsForLs.at(line).size() - spaceForDataOnLs.at(line);

        if (textForLs.at(line).size() > spaceForText) { // it's a scrolling line

            // set up the scrolling part
            textForScrollingLines.push_back(textForLs.at(line));
            scrollingLineNums.push_back(line+1);
            endSpotsForScrollingLines.push_back(spaceForText);

            // set up the static text for after the scrolling part
            string staticText = "";
            for (size_t x = 0; x < spaceForDataOnLs.at(line); x++) {
                staticText.append(" ");
            }
            staticText.append(labelsForLs.at(line));
            textForStaticLines.push_back(staticText);

            /// <TESTING>
            if (staticText.size() + spaceForText != 20) {
                cerr << "Error in LLSB ctor - line length incorrect for scrolling line " << line + 1 << endl;
            }
            /// </TESTING>
        }
        else { // it's a static line

            // add spaces to the (determined to be static) text (this is the space for data)
            for (size_t j = 0; j < spaceForDataOnLs.at(line); j++) {
                textForLs.at(line).append(" ");
            }

            textForLs.at(line).append(labelsForLs.at(line));
            textForStaticLines.push_back(textForLs.at(line));
            staticLineNums.push_back(line+1);

            /// <TESTING>
            if (textForLs.at(line).size() != 20) {
                cerr << "Error in LLSB ctor - line length incorrect for static line " << line + 1 << endl;
            }
            /// </TESTING>
        }
    }
}

void LabeledLineSetupBehavior::renderPage(IOHandler* iohandler) {

    iohandler->stopAllScrollingText();
    iohandler->printToLCD(titleLine, 0);

    std::vector<size_t> ss;
    for (size_t i = 0; i < scrollingLineNums.size(); i++) {
        ss.push_back(0); // start spots for scrolling lines
    }

    // this should start all the scrolling pieces at once
    iohandler->startScrollText(ss, endSpotsForScrollingLines, scrollingLineNums, textForScrollingLines);

    /// these two loops should only ever execute a total of three times - distribution changes per instance
    // prints the remaining text on the scrolling lines
    for (size_t x = 0; x < scrollingLineNums.size(); x++) {
        iohandler->printToLCD(textForStaticLines.at(scrollingLineNums.at(x)-1), 20*scrollingLineNums.at(x) + endSpotsForScrollingLines.at(x) + 1);
    }

    // prints the entire line for the non-scrolling lines
    for (size_t y = 0; y < staticLineNums.size(); y++) {
        iohandler->printToLCD(textForStaticLines.at(staticLineNums.at(y)-1), 20*staticLineNums.at(y));
    }

}


void LabeledLineSetupBehavior::updateLine(IOHandler* iohandler, size_t lineNum, string info) {

    if (lineNum != 1 && lineNum != 2 && lineNum != 3) {
        cerr << "Invalid line number passed to LabeledLineSetupBehavior::updateLine" << endl;
        return ;
    }

    info = info.substr(0, spaceForDataOnLines.at(lineNum-1));

    string output = "";

    while (output.size() < spaceForDataOnLines.at(lineNum - 1) - info.size() ) {
        // add necessary spaces to print over the entire updateable zone
        output.append(" ");
    }

    output.append(info);
    // prints info to updateSpotForLine(lineNum), and deals with update spots being right justified
    iohandler->printToLCD(info, 20*lineNum + 20 - textForStaticLines.at(lineNum-1).size());

}




