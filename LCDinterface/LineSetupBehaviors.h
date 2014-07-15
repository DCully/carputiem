#ifndef LINESETUPBEHAVIORS_H
#define LINESETUPBEHAVIORS_H

#include <vector>
#include <string>
#include "../Observer.h"
#include <iostream>


class IOHandler;

class LineSetupBehavior
{

    public:

        LineSetupBehavior();
        virtual ~LineSetupBehavior() {}
        LineSetupBehavior(std::vector<std::string> textForLines, const std::string& pageTitle);

        virtual LineSetupBehavior* clone() const { return new LineSetupBehavior(*this); }

        // called by controller
        virtual void renderPage(IOHandler& iohandler);
        virtual void updateLine(IOHandler* iohandler, ObserverPacket& obsp);
    protected:
        std::string titleLine;

        std::vector<size_t> staticLineNums;
        std::vector<std::string> textForStaticLines;

        std::vector<size_t> scrollingLineNums;
        std::vector<std::string> textForScrollingLines;
};

class LabeledLineSetupBehavior: public virtual LineSetupBehavior
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

        void renderPage(IOHandler* iohandler);
        void updateLine(IOHandler* iohandler, ObserverPacket& obsp);
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
        void updateLine(IOHandler* iohandler, ObserverPacket& obsp);
        DrawerLineSetupBehavior* clone() const { return new DrawerLineSetupBehavior(*this); }
};

/// ----------- for music screens -------------------

class SongListLineSetupBehavior : public LineSetupBehavior
{
    public:
        std::string artistAlbum;
        std::string songName;
        void renderPage(IOHandler& iohandler); // prints a blank for the song name
        void updateSong(const std::string& song); // fills in the song name (called by ScreenData)
        void updateLine(IOHandler* ioh, ObserverPacket& obsp) {/* don't do anything */}
        virtual SongListLineSetupBehavior* clone() const { return new SongListLineSetupBehavior(*this); }
    private:
        IOHandler* iohandler;
        void printSong();
};

class NowPlayingLineSetupBehavior: public LineSetupBehavior
{
    public:
        Song currentSong;
        void renderPage(IOHandler& iohandler);
        void updateLine(IOHandler* ioh, MusicObserverPacket& obsp); // print a new song
        virtual NowPlayingLineSetupBehavior* clone() const { return new NowPlayingLineSetupBehavior(*this); }
    private:
        void printTitleAndControls(IOHandler& ioh);
        void printArtistAndSong(IOHandler& ioh);
};

#endif
