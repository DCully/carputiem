#ifndef LINESETUPBEHAVIORS_H
#define LINESETUPBEHAVIORS_H

#include <vector>
#include <string>
#include "../Observer.h"
#include <iostream>


class IOHandlerInterface;

/*
* These classes encapsulate printing to the LCD for ScreenData objects.
* Every ScreenData owns one.
*/

class LineSetupBehavior
{

    public:

        LineSetupBehavior();
        virtual ~LineSetupBehavior() {}
        LineSetupBehavior(std::vector<std::string> textForLines, const std::string& pageTitle);

        virtual LineSetupBehavior* clone() const { return new LineSetupBehavior(*this); }

        // called by controller
        virtual void renderPage(IOHandlerInterface& iohandler);
        virtual void updateLine(IOHandlerInterface& iohandler, ObserverPacket& obsp);
    protected:
        std::string titleLine;

        std::vector<size_t> staticLineNums;
        std::vector<std::string> textForStaticLines;

        std::vector<size_t> scrollingLineNums;
        std::vector<std::string> textForScrollingLines;
};

class LabeledLineSetupBehavior: public LineSetupBehavior
{
    public:
        LabeledLineSetupBehavior();
        virtual ~LabeledLineSetupBehavior() {}
        LabeledLineSetupBehavior(
            std::vector<std::string> textForLines,
            std::vector<std::string> labelsForLines,
            std::vector<size_t> spaceForDataOnLines,
            const std::string& pageTitle);

        LabeledLineSetupBehavior* clone() const { return new LabeledLineSetupBehavior(*this); }

        void renderPage(IOHandlerInterface& iohandler);
        void updateLine(IOHandlerInterface& iohandler, ObserverPacket& obsp);
    protected:
        std::vector<size_t> endSpotsForScrollingLines;
        std::vector<size_t> spaceForDataOnLines;
        std::vector<size_t> updateSpotsForLines;

};

class DrawerLineSetupBehavior : public LineSetupBehavior
{
    public:
        DrawerLineSetupBehavior(std::vector<std::string> textForLines,
                                const std::string& pageTitle);
        void updateLine(IOHandlerInterface& iohandler, ObserverPacket& obsp);
        DrawerLineSetupBehavior* clone() const { return new DrawerLineSetupBehavior(*this); }
};

/// ----------- for music screens -------------------

class SongListLineSetupBehavior : public LineSetupBehavior
{
    public:
        void renderPage(IOHandlerInterface& iohandler); // prints a blank for the song name
        void updateSong(const Song& song, IOHandlerInterface& ioh); // fills in the song name (called by ScreenData)
        void updateLine(IOHandlerInterface& ioh, ObserverPacket& obsp) {/* don't do anything */}
        virtual SongListLineSetupBehavior* clone() const { return new SongListLineSetupBehavior(*this); }
    private:
        Song currentSong;
        void printSong(IOHandlerInterface& ioh);
};

class NowPlayingLineSetupBehavior: public LineSetupBehavior
{
    public:
        Song currentSong;
        void renderPage(IOHandlerInterface& iohandler);
        void updateLine(IOHandlerInterface& ioh, ObserverPacket& obsp); // print a new song
        virtual NowPlayingLineSetupBehavior* clone() const { return new NowPlayingLineSetupBehavior(*this); }
    private:
        void printTitleAndControls(IOHandlerInterface& ioh);
        void printArtistAndSong(IOHandlerInterface& ioh);
};

#endif
