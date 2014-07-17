#include "LineSetupBehaviors.h"
#include <iostream>
#include "IOHandler.h"
#include "../Observer.h"

/// for base class (one static or scrolled string per line)

LineSetupBehavior::LineSetupBehavior(std::vector<std::string> textForLines, const std::string& t)
{
    titleLine = t.substr(0,15);
    while (titleLine.size() < 17) {
        titleLine.append(" ");
    }
    titleLine = titleLine + "<^>";

    for (size_t x = 1; x < 4 && x < textForLines.size() + 1; x++) { // loop through textForLines and pull out scrolling ones
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

void LineSetupBehavior::renderPage(IOHandlerInterface& iohandler) {
    iohandler.stopAllScrollingText();
    iohandler.printToLCD(titleLine, 0);

    // print the static lines
    for (size_t x = 0; x < textForStaticLines.size(); x++) {
        iohandler.printToLCD(textForStaticLines.at(x), staticLineNums.at(x)*20);
    }

    // start the scrolling thread for dynamic lines (they all take up the whole line, no need to clear)
    if (textForScrollingLines.size() > 0) {
        std::vector<size_t> ss;
        std::vector<size_t> es;

        for (size_t i = 0; i < textForScrollingLines.size(); i++) {
            ss.push_back(0);
            es.push_back(19);
        }

        iohandler.startScrollText(ss, es, scrollingLineNums, textForScrollingLines);
    }
}

void LineSetupBehavior::updateLine(IOHandlerInterface* iohandler, ObserverPacket& obsp) {

    std::string info = ( (ObdObserverPacket*) &obsp)->information;
    int lineNum = ( (ObdObserverPacket*) &obsp)->linenum;

    if (lineNum != 1 && lineNum != 2 && lineNum != 3) {
        std::cerr << "Invalid line number passed to LineSetupBehavior::updateLine" << std::endl;
        return ;
    }

    // find where this lineNum is in the two line text arrays and update that spot, then reprint page
    for (size_t x = 0; x < scrollingLineNums.size(); x++) {
        if (scrollingLineNums.at(x) == (size_t) lineNum) {
            textForScrollingLines.at(x) = info;
            return ;
        }
    }

    for (size_t y = 0; y < staticLineNums.size(); y++) {
        if (staticLineNums.at(y) == (size_t) lineNum) {
            textForStaticLines.at(y) = info;
            return ;
        }
        else {
            std::cerr << "Error in LSB::updateLine - lineNum " << lineNum << " not found in either lineNums vector" << std::endl;
            return ;
        }
    }

    renderPage(*iohandler);

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

    for (size_t s = 0; s < spaceForDataOnLs.size(); s++) {
        spaceForDataOnLines.push_back(spaceForDataOnLs.at(s)+1); // it's a cheesy fix, but it works
    }
    // set up title line
    titleLine = t.substr(0,15);
    while (titleLine.size() < 17) {
        titleLine.append(" ");
    }
    titleLine.append("<^>");

    // determine body lines setup
    for (size_t line = 0; line < 3 && line < textForLs.size(); line++) {

        size_t spaceForText = 20 - labelsForLs.at(line).size() - spaceForDataOnLines.at(line);
        updateSpotsForLines.push_back(spaceForText);
        if (textForLs.at(line).size() > spaceForText) { // it's a scrolling line

            // set up the scrolling part
            textForScrollingLines.push_back(textForLs.at(line));
            scrollingLineNums.push_back(line+1);
            endSpotsForScrollingLines.push_back(spaceForText);

            // set up the static text for after the scrolling part
            std::string staticText = "";
            for (size_t x = 0; x < spaceForDataOnLines.at(line); x++) {
                staticText.append(" ");
            }
            staticText.append(labelsForLs.at(line));
            textForStaticLines.push_back(staticText);

            /// <TESTING>
            if (staticText.size() + spaceForText != 20) {
                std::cerr << "Error in LLSB ctor - line length incorrect for scrolling line " << line + 1 << std::endl;
            }
            /// </TESTING>
        }
        else { // it's a static line
            // add spaces to the (determined to be static) text (this is the space for data AND to fill the rest of the text slot)
            while (textForLs.at(line).size() < 20 - labelsForLs.at(line).size()) {
                textForLs.at(line).append(" ");
            }
            textForLs.at(line).append(labelsForLs.at(line));
            textForStaticLines.push_back(textForLs.at(line));
            staticLineNums.push_back(line+1);

            /// <TESTING>
            if (textForLs.at(line).size() != 20) {
                std::cerr << "Error in LLSB ctor - line length incorrect for static line " << line + 1 << std::endl;
            }
            /// </TESTING>
        }
    }
}

void LabeledLineSetupBehavior::renderPage(IOHandlerInterface* iohandler) {

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
        iohandler->printToLCD(textForStaticLines.at(scrollingLineNums.at(x)-1), 20*scrollingLineNums.at(x) + endSpotsForScrollingLines.at(x));
    }

    // prints the entire line for the non-scrolling lines
    for (size_t y = 0; y < staticLineNums.size(); y++) {
        iohandler->printToLCD(textForStaticLines.at(staticLineNums.at(y)-1), 20*staticLineNums.at(y));
    }

}


void LabeledLineSetupBehavior::updateLine(IOHandlerInterface* iohandler, ObserverPacket& obsp) {

    std::string info = ( (ObdObserverPacket*) &obsp)->information;
    int lineNum = ( (ObdObserverPacket*) &obsp)->linenum;

    if (lineNum != 1 && lineNum != 2 && lineNum != 3) {
        std::cerr << "Invalid line number passed to LabeledLineSetupBehavior::updateLine" << std::endl;
        return ;
    }

    info = info.substr(0, spaceForDataOnLines.at(lineNum-1));

    std::string output = "";

    while (output.size() < spaceForDataOnLines.at(lineNum - 1) - info.size() ) {
        // add necessary spaces to print over the entire updateable zone
        output.append(" ");
    }

    output.append(info);

    // prints info to updateSpotForLine(lineNum), and deals with update spots being right justified
    iohandler->printToLCD(output, 20*lineNum + updateSpotsForLines.at(lineNum-1));

}



/// -------------------DrawerLineSetupBehavior-----------------------

DrawerLineSetupBehavior::DrawerLineSetupBehavior(std::vector<std::string> drawerNames,
                                                 const std::string& pageTitle)
{
    titleLine = pageTitle.substr(0,16);
    while (titleLine.size() < 17) {
        titleLine.append(" ");
    }
    titleLine.append("<^>");

    // format the drawer lines within drawerNames
    for (size_t x = 0; x < drawerNames.size(); ++x) {
        drawerNames.at(x) = drawerNames.at(x).substr(0, 17);
        while (drawerNames.at(x).size() < 19) {
            drawerNames.at(x).append(" ");
        }
        drawerNames.at(x).append(">");
        textForStaticLines.push_back(drawerNames.at(x));
    }

    while (textForStaticLines.size() < 3) {
        textForStaticLines.push_back("                   >");
    }
    staticLineNums.push_back(1);
    staticLineNums.push_back(2);
    staticLineNums.push_back(3);
}

void DrawerLineSetupBehavior::updateLine(IOHandlerInterface* ioh, ObserverPacket& obsp) {
    // dont do anything
}


/// ------------ SongListLineSetupBehavior ------------------

void SongListLineSetupBehavior::renderPage(IOHandlerInterface& ioh)
{
    // title line
    ioh.printToLCD("Song List        <^>", 0);
    // print the artist from the current song
    ioh.printToLCD(currentSong.artistName.substr(0, 20), 20);
    // print blank for line 2 (for individual song
    ioh.printToLCD(" <                > ", 40);
    // controls line
    ioh.printToLCD("Play this one: --->*", 60);
    // call printSong to print the actual song title
    printSong(ioh);
}

void SongListLineSetupBehavior::updateSong(const Song& song, IOHandlerInterface& ioh)
{
    currentSong = song;
    printSong(ioh);
}

void SongListLineSetupBehavior::printSong(IOHandlerInterface& ioh)
{
    std::string trackname = currentSong.trackName;
    if (trackname.size() <=16) {
        while (trackname.size() < 16) {
            trackname.append(" ");
        }
        ioh.printToLCD(trackname, 42);
    }
    else {
        std::vector<size_t> startspots;
        std::vector<size_t> stopspots;
        std::vector<size_t> linenums;
        std::vector<std::string> scrollingtext;

        startspots.push_back(42);
        stopspots.push_back(57);
        linenums.push_back(2);
        scrollingtext.push_back(trackname);
        ioh.startScrollText(startspots, stopspots, linenums, scrollingtext);
    }
}

/// -------------- NowPlayingLineSetupBehavior -------------------

void NowPlayingLineSetupBehavior::renderPage(IOHandlerInterface& ioh)
{
    printTitleAndControls(ioh);
    printArtistAndSong(ioh);
}

void NowPlayingLineSetupBehavior::updateLine(IOHandlerInterface* ioh, ObserverPacket& obsp)
{
    // observer packet should have a new song in it
    currentSong = ( (MusicObserverPacket*) &obsp)->currentSong;
    printArtistAndSong(*ioh);
}

void NowPlayingLineSetupBehavior::printTitleAndControls(IOHandlerInterface& ioh)
{
    // title line
    ioh.printToLCD("Now Playing      <^>", 0);

    // controls line
    ioh.printToLCD(" Pause: *  Vol: +/- ", 60);
}

void NowPlayingLineSetupBehavior::printArtistAndSong(IOHandlerInterface& ioh)
{
    std::vector<size_t> startspots;
    std::vector<size_t> stopspots;
    std::vector<size_t> linenums;
    std::vector<std::string> scrollingtext;

    // artist and album line
    std::string artistAndAlbum = currentSong.artistName + " - " + currentSong.albumName;
    if (artistAndAlbum.size() <= 20) {
        // static case
        while (artistAndAlbum.size() < 20) {
            artistAndAlbum.append(" ");
        }
        ioh.printToLCD(artistAndAlbum, 20);
    }
    else {
        // scrolling case
        startspots.push_back(0);
        stopspots.push_back(19);
        linenums.push_back(1);
        scrollingtext.push_back(artistAndAlbum);
    }

    // song name line
    std::string trackname = currentSong.trackName;
    if (trackname.size() <= 20) {
        // static case
        while (trackname.size() < 20) {
            trackname.append(" ");
        }
        ioh.printToLCD(trackname, 40);
    }
    else {
        // scrolling case
        startspots.push_back(0);
        stopspots.push_back(19);
        linenums.push_back(2);
        scrollingtext.push_back(trackname);
    }

    if (scrollingtext.size() > 0) {
        ioh.startScrollText(startspots, stopspots, linenums, scrollingtext);
    }
}


