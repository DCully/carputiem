#include "MusicScreenData.h"

///------------ArtistOrAlbumScreenData-------------

ArtistOrAlbumScreenData::ArtistOrAlbumScreenData(MusicManager* obs,
                                                PageChangeBehavior* pcb,
                                                DrawerLineSetupBehavior* lsb,
                                                const std::string& drawer1key,
                                                const std::string& drawer2key,
                                                const std::string& drawer3key)
: ScreenDataDrawer(obs, pcb, lsb), keyForDrawer1(drawer1key), keyForDrawer2(drawer2key), keyForDrawer3(drawer3key)
{
    // do nothing - this class just needs to enforce having a MusicManager observable for doLeavePageBehavior
}

void ArtistOrAlbumScreenData::doLeavePageBehavior()
{
    // if leaving by 39, 59, or 79, update the current song subset in the MusicManager
    switch (cursorSpots.at(currentSpotIndex)) {
        case (39):
            ((MusicManager*) observed)->setCurrentlyBrowsedSongSubset(keyForDrawer1);
            break;
        case (59):
            ((MusicManager*) observed)->setCurrentlyBrowsedSongSubset(keyForDrawer2);
            break;
        case (79):
            ((MusicManager*) observed)->setCurrentlyBrowsedSongSubset(keyForDrawer3);
            break;
    }
}

///------------SongListScreenData--------------

SongListScreenData::SongListScreenData(MusicManager* obs, PageChangeBehavior* pcb, SongListLineSetupBehavior* lsb)
: ScreenData(obs, pcb, lsb)
{
    // add spots for flipping through songs and for selecting one to play
    cursorSpots.push_back(41);
    cursorSpots.push_back(58);
    cursorSpots.push_back(79);
}

void SongListScreenData::doLoadPageBehavior()
{
    // getCurrentSongSubset
    songSelectionEndpointIterators = ((MusicManager*) observed)->getCurrentlyBrowsedSongSubset();
    currentSongIterator = songSelectionEndpointIterators.first;
    ((SongListLineSetupBehavior*)lineSetupBehavior)->updateSong(currentSongIterator->second.trackName);
    ((SongListLineSetupBehavior*)lineSetupBehavior)->artistAlbum = currentSongIterator->second.artistName;
}

void SongListScreenData::doLeavePageBehavior()
{
    // if leaving via "play" button, change what song is playing as you leave...
    if (cursorSpots.at(currentSpotIndex) == 79) {
        ((MusicManager*) observed)->playSong(currentSongIterator);
    }
}

void SongListScreenData::doCurSpotSelectBehavior() {
    if (cursorSpots.at(currentSpotIndex) == 41) {
        // update the currentSongSelectionIterator
        if (--currentSongIterator != songSelectionEndpointIterators.first) {
            --currentSongIterator;
        }
        // call LSBs update line with name of the previous song
        ((SongListLineSetupBehavior*)lineSetupBehavior)->updateSong(currentSongIterator->second.trackName);

    }
    else if (cursorSpots.at(currentSpotIndex) == 58) {
        // update the currentSongSelectionIterator
        if (++currentSongIterator != songSelectionEndpointIterators.second) {
            ++currentSongIterator;
        }
        // call LSBs update line with name of the next song
        ((SongListLineSetupBehavior*)lineSetupBehavior)->updateSong(currentSongIterator->second.trackName);
    }
}

///------------NowPlayingScreenData--------------

NowPlayingScreenData::NowPlayingScreenData(MusicManager* obs, PageChangeBehavior* pcb, NowPlayingLineSetupBehavior* lsb)
{
    cursorSpots.push_back(68); // play/pause
    cursorSpots.push_back(76); // volume up
    cursorSpots.push_back(78); // volume down
}

void NowPlayingScreenData::doLoadPageBehavior()
{
    // getCurrentSong and pass it to the LSB
    ((NowPlayingLineSetupBehavior*)lineSetupBehavior)->currentSong = ((MusicManager*) observed)->getCurrentSong();
}

void NowPlayingScreenData::doCurSpotSelectBehavior() {
    int spot = cursorSpots.at(currentSpotIndex);
    switch (spot) {
        case 68:
            ((MusicManager*) observed)->togglePause();
            break;
        case 76:
            ((MusicManager*) observed)->increaseVolume();
            break;
        case 78:
            ((MusicManager*) observed)->decreaseVolume();
            break;
    }
}
