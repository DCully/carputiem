#ifndef MUSICSCREENDATA_H
#define MUSICSCREENDATA_H

#include "ScreenData.h"
#include "../MusicPlayer/MusicManager.h"
#include "ScreenDataDrawer.h"

class ArtistOrAlbumScreenData: public ScreenData
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
        virtual ArtistOrAlbumScreenData* clone() const {return new ArtistOrAlbumScreenData(*this); }
        ArtistOrAlbumScreenData(const ArtistOrAlbumScreenData& other);
        ArtistOrAlbumScreenData& operator=(ArtistOrAlbumScreenData other);
        virtual void swap(ArtistOrAlbumScreenData& other);
        void doLeavePageBehavior();
    private:
        /// these need to be artist names or artist+album names
        std::string keyForDrawer1;
        std::string keyForDrawer2;
        std::string keyForDrawer3;
};

class SongListScreenData: public ScreenData
{
    public:
        SongListScreenData(MusicManager* obs, PageChangeBehavior* pcb, SongListLineSetupBehavior* lsb);
        void doLoadPageBehavior(IOHandlerInterface& ioh);
        void doLeavePageBehavior();
        void doCurSpotSelectBehavior(IOHandlerInterface& ioh);
        SongListScreenData(const SongListScreenData& other);
        SongListScreenData& operator=(SongListScreenData other);
        virtual void swap(SongListScreenData& other);
        virtual SongListScreenData* clone() const {return new SongListScreenData(*this); }
    private:
        std::vector<std::string> songList;
        size_t curSongIndex;
};

class NowPlayingScreenData: public ScreenData
{
    public:
        NowPlayingScreenData(MusicManager* obs, PageChangeBehavior* pcb, NowPlayingLineSetupBehavior* lsb);
        NowPlayingScreenData(const NowPlayingScreenData& other);
        NowPlayingScreenData& operator=(NowPlayingScreenData other);
        virtual void swap(NowPlayingScreenData& other);
        virtual NowPlayingScreenData* clone() const {return new NowPlayingScreenData(*this); }
        void doLoadPageBehavior(IOHandlerInterface& ioh);
        void doCurSpotSelectBehavior(IOHandlerInterface& ioh);
};

#endif
