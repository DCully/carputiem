#include "MusicScreenFactory.h"
#include "../LCDinterface/ScreenDataDrawer.h"
#include <vector>
#include <set>
#include <string>

/*
    Builds all the screens to hook up a MusicManager to a ScreenDataManager.
    This assumes that the MusicManager has already read in all of the songs from files.
*/

void MusicScreenFactory::buildScreens(MusicManager& mm, ScreenDataManager& scdm)
{
    /// add main drawer for music branch
    std::vector<std::string> textforlines;
    textforlines.push_back("Artists");
    textforlines.push_back("Albums");
    textforlines.push_back("Now Playing");
    DrawerLineSetupBehavior* dlsb = new DrawerLineSetupBehavior(textforlines, "Music");
    PageChangeBehavior* pcb = new PageChangeBehavior();
    ScreenDataDrawer* musicmain = new ScreenDataDrawer(&scdm, pcb, dlsb);
    scdm.addScreens(musicmain, "MusicMain", "home", 1);


    /// build drawers for artists
    std::set<std::string> artists = mm.getArtistSet();
    std::vector<ScreenData*> artistScreens;

    // each outer loop is one screen
    for (auto it = artists.begin(); it != artists.end();)
    {
        std::vector<std::string> text;
        ArtistOrAlbumScreenData* screen;
        // each inner loop is one artist from the iterator
        for (int x = 0; x < 3 && it != artists.end(); ++x)
        {
            text.push_back(*it);
            ++it;
        }

        while (text.size() < 3) {
            text.push_back(" ");
        }

        screen = new ArtistOrAlbumScreenData(&mm, new PageChangeBehavior(), new DrawerLineSetupBehavior(text, "Artists"), text.at(0), text.at(1), text.at(2));
        artistScreens.push_back(screen);

        if (it == artists.end()) {
            break;
        }
    }

    /// add artist screens to music drawer
    scdm.addScreens(artistScreens, "ArtistScreens", "MusicMain", 1);


    /// build drawers for albums
    std::set<std::string> albums = mm.getAlbumSet();
    std::vector<ScreenData*> albumScreens;

    // each outer loop is one screen
    for (auto it = albums.begin(); it != albums.end();)
    {
        std::vector<std::string> text;
        ArtistOrAlbumScreenData* screen;
        // each inner loop is one artist from the iterator
        for (int x = 0; x < 3 && it != artists.end(); ++x)
        {
            text.push_back(*it);
            ++it;
        }

        while (text.size() < 3) {
            text.push_back(" ");
        }

        screen = new ArtistOrAlbumScreenData(&mm, new PageChangeBehavior(), new DrawerLineSetupBehavior(text, "Albums"), text.at(0), text.at(1), text.at(2));
        albumScreens.push_back(screen);

        if (it == albums.end()) {
            break;
        }
    }

    /// add album screens to music drawer
    scdm.addScreens(albumScreens, "AlbumScreens", "MusicMain", 2);

    /// build Now Playing screen and add it
    NowPlayingScreenData* npsd = new NowPlayingScreenData(&mm, new PageChangeBehavior(), new NowPlayingLineSetupBehavior());
    std::vector<ScreenData*> invec;
    invec.push_back(npsd);
    scdm.addScreens(invec, "NowPlaying", "MusicMain", 3);

    /// build the Song List screen and add it
    SongListScreenData* slsd = new SongListScreenData(&mm, new PageChangeBehavior(), new SongListLineSetupBehavior());
    std::vector<ScreenData*> invec2;
    invec2.push_back(slsd);
    scdm.addScreens(invec2, "SongList", "AlbumScreens", 2);
}
