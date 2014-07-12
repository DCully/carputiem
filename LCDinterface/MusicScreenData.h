#ifndef MUSICSCREENDATA_H
#define MUSICSCREENDATA_H

#include "ScreenData.h"
#include "../MusicPlayer/MusicManager.h"
#include "ScreenDataDrawer.h"

class ArtistOrAlbumScreenData: public ScreenDataDrawer
{
    public:
        /// the drawerkeys are the artists or artist+albums that each drawer leads to
        /// when leaving the page via drawer, this object will tell the MusicManager
        /// which artist or artist+album you've gone down to
        ArtistOrAlbumScreenData(MusicManager* obs,
                                PageChangeBehavior* pcb,
                                DrawerLineSetupBehavior* lsb,
                                const std::string& drawer1key,
                                const std::string& drawer2key,
                                const std::string& drawer3key);
        void doLeavePageBehavior();
    private:
        std::string keyForDrawer1;
        std::string keyForDrawer2;
        std::string keyForDrawer3;
};

class SongListScreenData: public ScreenData
{
    public:
        SongListScreenData(MusicManager* obs, PageChangeBehavior* pcb, SongListLineSetupBehavior* lsb);
        void doLoadPageBehavior();
        void doLeavePageBehavior();
        void doCurSpotSelectBehavior();
    private:
        std::pair<std::map<std::string, Song>::iterator, std::map<std::string, Song>::iterator> songSelectionEndpointIterators;
        std::map<std::string, Song>::iterator currentSongIterator;
};

class NowPlayingScreenData: public ScreenData
{
    public:
        NowPlayingScreenData(MusicManager* obs, PageChangeBehavior* pcb, NowPlayingLineSetupBehavior* lsb);
        void doLoadPageBehavior();
        void doCurSpotSelectBehavior();
};

#endif
