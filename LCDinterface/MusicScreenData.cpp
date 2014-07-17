#include "MusicScreenData.h"

///------------ArtistOrAlbumScreenData-------------

ArtistOrAlbumScreenData::ArtistOrAlbumScreenData(MusicManager* obs,
                                                PageChangeBehavior* pcb,
                                                DrawerLineSetupBehavior* lsb,
                                                const std::string& drawer1key,
                                                const std::string& drawer2key,
                                                const std::string& drawer3key)
: ScreenData(obs, pcb, lsb), keyForDrawer1(drawer1key), keyForDrawer2(drawer2key), keyForDrawer3(drawer3key)
{
    cursorSpots.push_back(39);
    cursorSpots.push_back(59);
    cursorSpots.push_back(79);
}

ArtistOrAlbumScreenData::ArtistOrAlbumScreenData(const ArtistOrAlbumScreenData& other)
{
    observed = other.observed;
    pageChangeBehavior = other.pageChangeBehavior->clone();
    lineSetupBehavior = other.lineSetupBehavior->clone();
    cursorSpots = other.cursorSpots;
    currentSpotIndex = other.currentSpotIndex;
    keyForDrawer1 = other.keyForDrawer1;
    keyForDrawer2 = other.keyForDrawer2;
    keyForDrawer3 = other.keyForDrawer3;
}

ArtistOrAlbumScreenData& ArtistOrAlbumScreenData::operator=(ArtistOrAlbumScreenData other) {
    ArtistOrAlbumScreenData temp(other);
    swap(temp);
    return *this;
}

void ArtistOrAlbumScreenData::swap(ArtistOrAlbumScreenData& other) {
    std::swap(observed, other.observed);
    std::swap(lineSetupBehavior, other.lineSetupBehavior);
    std::swap(pageChangeBehavior, other.pageChangeBehavior);
    std::swap(currentSpotIndex, other.currentSpotIndex);
    std::swap(cursorSpots, other.cursorSpots);
    std::swap(keyForDrawer1, other.keyForDrawer1);
    std::swap(keyForDrawer2, other.keyForDrawer2);
    std::swap(keyForDrawer3, other.keyForDrawer3);
}

void ArtistOrAlbumScreenData::doLeavePageBehavior()
{
    // if leaving by 39, 59, or 79, update the current song subset in the MusicManager
    switch (cursorSpots.at(currentSpotIndex)) {
        case (39):
            ((MusicManager*) observed)->setCurrentSongSubset(keyForDrawer1);
            break;
        case (59):
            ((MusicManager*) observed)->setCurrentSongSubset(keyForDrawer2);
            break;
        case (79):
            ((MusicManager*) observed)->setCurrentSongSubset(keyForDrawer3);
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
    curSongIndex = 0;
}

void SongListScreenData::doLoadPageBehavior(IOHandlerInterface& ioh)
{
    /// songList should be a vector of complete keys
    songList = ((MusicManager*) observed)->getCurrentSongSubset();
    curSongIndex = 0;

    /// pass song from current key to LSB
    Song song = ((MusicManager*) observed)->getSongByKey(songList.at(curSongIndex));
    ((SongListLineSetupBehavior*) lineSetupBehavior)->updateSong(song, ioh);
}

void SongListScreenData::doLeavePageBehavior()
{
    /// if leaving via "play" button, change what song is playing as you leave...
    if (cursorSpots.at(currentSpotIndex) == 79) {
        ((MusicManager*) observed)->playSong(songList.at(curSongIndex));
    }
}

void SongListScreenData::doCurSpotSelectBehavior(IOHandlerInterface& ioh) {
    if (cursorSpots.at(currentSpotIndex) == 41) {
        // update the currentSongSelectionIterator
        if (curSongIndex > 0) {
            --curSongIndex;
        }
        // call LSBs update line with name of the song
        ((SongListLineSetupBehavior*)lineSetupBehavior)->updateSong( ((MusicManager*) observed)->getSongByKey(songList.at(curSongIndex)), ioh );

    }
    else if (cursorSpots.at(currentSpotIndex) == 58) {
        // update the currentSongSelectionIterator
        // update the currentSongSelectionIterator
        if (curSongIndex < songList.size() - 1) {
            ++curSongIndex;
        }
        // call LSBs update line with name of the next song
        ((SongListLineSetupBehavior*)lineSetupBehavior)->updateSong( ((MusicManager*) observed)->getSongByKey(songList.at(curSongIndex)), ioh );
    }
}

SongListScreenData::SongListScreenData(const SongListScreenData& other) {
    observed = other.observed;
    pageChangeBehavior = other.pageChangeBehavior->clone();
    lineSetupBehavior = other.lineSetupBehavior->clone();
    cursorSpots = other.cursorSpots;
    currentSpotIndex = other.currentSpotIndex;
    songList = other.songList;
    curSongIndex = other.curSongIndex;
}

SongListScreenData& SongListScreenData::operator=(SongListScreenData other) {
    SongListScreenData temp(other);
    swap(temp);
    return *this;
}

void SongListScreenData::swap(SongListScreenData& other) {
    std::swap(observed, other.observed);
    std::swap(lineSetupBehavior, other.lineSetupBehavior);
    std::swap(pageChangeBehavior, other.pageChangeBehavior);
    std::swap(currentSpotIndex, other.currentSpotIndex);
    std::swap(cursorSpots, other.cursorSpots);
    std::swap(songList, other.songList);
    std::swap(curSongIndex, other.curSongIndex);
}

///------------NowPlayingScreenData--------------

NowPlayingScreenData::NowPlayingScreenData(MusicManager* obs, PageChangeBehavior* pcb, NowPlayingLineSetupBehavior* lsb)
: ScreenData(obs, pcb, lsb)
{
    cursorSpots.push_back(68); // play/pause
    cursorSpots.push_back(76); // volume up
    cursorSpots.push_back(78); // volume down
}

void NowPlayingScreenData::doLoadPageBehavior(IOHandlerInterface& ioh)
{
    // getCurrentSong and pass it to the LSB
    Song song = ((MusicManager*) observed)->getCurrentSong();
    ((NowPlayingLineSetupBehavior*)lineSetupBehavior)->currentSong = song;
}

void NowPlayingScreenData::doCurSpotSelectBehavior(IOHandlerInterface& ioh) {
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

NowPlayingScreenData::NowPlayingScreenData(const NowPlayingScreenData& other)
{
    observed = other.observed;
    cursorSpots = other.cursorSpots;
    currentSpotIndex = other.currentSpotIndex;
    lineSetupBehavior = other.lineSetupBehavior->clone();
    pageChangeBehavior = other.pageChangeBehavior->clone();
}

NowPlayingScreenData& NowPlayingScreenData::operator=(NowPlayingScreenData other)
{
    NowPlayingScreenData temp(other);
    swap(temp);
    return *this;
}

void NowPlayingScreenData::swap(NowPlayingScreenData& other)
{
    std::swap(observed, other.observed);
    std::swap(lineSetupBehavior, other.lineSetupBehavior);
    std::swap(pageChangeBehavior, other.pageChangeBehavior);
    std::swap(currentSpotIndex, other.currentSpotIndex);
    std::swap(cursorSpots, other.cursorSpots);
}


