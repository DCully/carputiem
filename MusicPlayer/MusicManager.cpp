#include "MusicManager.h"
#include "dirent.h"
#include "mpg123.h"
#include "unistd.h"
#include <ao/ao.h>
#include "dirent.h"
#include "sys/types.h"
#include <stdlib.h>
#include <utility>
#include <chrono>
#include <thread>
#include <fileref.h>
#include <tag.h>
#include <tpropertymap.h>

volatile bool MusicManager::run = true;
std::mutex MusicManager::runLock;

MusicManager::MusicManager(const std::string& musicDirPath)
: musicDirectory(musicDirPath)
{
    // start up libao
    ao_initialize();

    // start up mpg123 with default settings
    mpg123_init();

    // read the music library into memory
    std::vector<std::string> songFilePaths = getRawMp3FileNames();
    for (size_t x = 0; x < songFilePaths.size(); ++x) {
        addSong(songFilePaths.at(x));
    }

    std::unique_lock<std::mutex> slock(songLock);
    slock.lock();
    currentSongIterator = songMap.begin();
    slock.unlock();

    std::thread(&MusicManager::play, this, musicDirectory);

    keyForCurrentlyBrowsedSongSubset = "";
}

MusicManager::~MusicManager()
{
    std::lock_guard<std::mutex> lock(runLock);
    run = false;
    /*
       this will eventually kill the thread, which will close down the library
       we don't need to do anything else here, because the other thread will
       only try to access member variables when it has the runLock, and will
       check the boolean variable "run" whenever it reacquires runLock
    */
}

void MusicManager::setCurrentSongIterator(const std::map<std::string, Song>::iterator it) {

    MusicObserverPacket packet;
    std::unique_lock<std::mutex> locker(songLock);
    locker.lock();
    currentSongIterator = it;
    locker.unlock();
    packet.currentSong = currentSongIterator->second;
    notifyObservers(packet);
}

const std::map<std::string, Song>::iterator MusicManager::getCurrentSongIterator() {
    std::lock_guard<std::mutex> lock(songLock);
    return currentSongIterator;
}

Song MusicManager::getCurrentSong() {
    std::lock_guard<std::mutex> lock(songLock);
    return (currentSongIterator->second);
}

std::pair<const std::map<std::string, Song>::iterator, const std::map<std::string, Song>::iterator> MusicManager::getSongsByArtist(const std::string& artistName)
{
    std::pair<const std::map<std::string, Song>::iterator, const std::map<std::string, Song>::iterator> myPair(songMap.lower_bound(artistName), songMap.upper_bound(artistName));
    return myPair;
}

std::pair<const std::map<std::string, Song>::iterator, const std::map<std::string, Song>::iterator> MusicManager::getSongsByArtistFromAlbum(const std::string& artistName, const std::string& albumName)
{
    std::pair<const std::map<std::string, Song>::iterator, const std::map<std::string, Song>::iterator> myPair(songMap.lower_bound(artistName+albumName), songMap.upper_bound(artistName+albumName));
    return myPair;
}

void MusicManager::playSong(const std::map<std::string, Song>::iterator it) {
    // lock the queue first, so the song thread doesnt read between iterator change and queue push
    std::lock_guard<std::mutex> lock(queueLock);
    std::lock_guard<std::mutex> lock2(songLock);
    currentSongIterator = it; // swap currentSong iterator to point to the new song
    taskQueue.push(1); // 1 changes the song
}

void MusicManager::increaseVolume() {
    std::lock_guard<std::mutex> lock(queueLock);
    taskQueue.push(3);
}

void MusicManager::decreaseVolume() {
    std::lock_guard<std::mutex> lock(queueLock);
    taskQueue.push(4);
}

void MusicManager::togglePause() {
    std::lock_guard<std::mutex> lock(queueLock);
    taskQueue.push(2);
}

// setup function
void MusicManager::addSong(const std::string& songFileName) {

    // get metadata from the file using taglib
    std::string artistName, albumName, trackName;
    int trackNumber;
    TagLib::FileRef f(songFileName.c_str());

    // data sanity
    if (f.tag()->artist() == TagLib::String::null) {
        std::cerr << "Song file named: " << songFileName << " had empty artist tag - not indexing it" << std::endl;
        return;
    }

    if (f.tag()->album() == TagLib::String::null) {
        std::cerr << "Song file named: " << songFileName << " had empty album tag - not indexing it" << std::endl;
        return;
    }

    if (f.tag()->title() == TagLib::String::null) {
        std::cerr << "Song file named: " << songFileName << " had empty title tag - not indexing it" << std::endl;
        return;
    }

    artistName = f.tag()->artist().to8Bit(true);
    albumName = f.tag()->album().to8Bit(true);
    trackNumber = f.tag()->track(); // returns 0 if tag is empty
    trackName = f.tag()->title().to8Bit(true);

    // add new song to songMap with appropriate key
    Song song(songFileName, artistName, albumName, trackNumber, trackName);
    std::string key = "";
    key.append(artistName);
    key.append(albumName);
    key.append(std::to_string(trackNumber));
    key.append(trackName);
    std::pair<std::string, Song> newPair1(key, song);
    songMap.insert(newPair1);

    // add artist/album pair to set
    artistSet.insert(artistName);
    albumSet.insert(albumName);
}

std::set<std::string> MusicManager::getArtistSet() {
    return artistSet;
}

std::set<std::string> MusicManager::getAlbumSet() {
    return albumSet;
}

void MusicManager::setCurrentlyBrowsedSongSubset(const std::string& key) {
    keyForCurrentlyBrowsedSongSubset = key;
}

std::pair<const std::map<std::string, Song>::iterator, const std::map<std::string, Song>::iterator> MusicManager::getCurrentlyBrowsedSongSubset() {
    std::pair<const std::map<std::string, Song>::iterator, const std::map<std::string, Song>::iterator> myPair(songMap.lower_bound(keyForCurrentlyBrowsedSongSubset), songMap.upper_bound(keyForCurrentlyBrowsedSongSubset));
    return myPair;
}

std::pair<const std::map<std::string, Song>::iterator, const std::map<std::string, Song>::iterator> MusicManager::getSongsByArtistFromAlbum(const std::string& artistPlusAlbumName) {
    std::pair<const std::map<std::string, Song>::iterator, const std::map<std::string, Song>::iterator> myPair(songMap.lower_bound(artistPlusAlbumName), songMap.upper_bound(artistPlusAlbumName));
    return myPair;
}

std::vector<std::string> MusicManager::getRawMp3FileNames() {

    dirent* directoryEntry;
    std::vector<std::string> mp3FileNames;

    DIR* directoryPtr = opendir(musicDirectory.c_str());
    if (directoryPtr != NULL) {
        while ( (directoryEntry = readdir(directoryPtr)) ) {
            std::string name = directoryEntry->d_name;
            if (name.size() > 4 && name.substr(name.size()-4,4)==".mp3") {
                mp3FileNames.push_back(name);
            }
        }
    }

    return mp3FileNames;
}

// runs in a separate thread, launched from playSong()
void MusicManager::play(std::string musicDirPath) {

    // for use with libraries
    long rate;
    int channels;
    int encoding;
    size_t bytesDone;
    size_t bufferSize;
    unsigned char* buffer;
    ao_sample_format format;
    int mpgNewError;
    int driverID = ao_default_driver_id();
    mpg123_handle* mpg123handle = mpg123_new(NULL, &mpgNewError);
    ao_device* aodevice;

    // for use with thread coordination
    std::string songPath;
    std::map<std::string, Song>::iterator songPlaying;
    std::unique_lock<std::mutex> rlock(runLock);
    std::unique_lock<std::mutex> qlock(queueLock);
    int task;
    volatile bool paused = false;
    volatile bool increaseVolume = false;
    volatile bool decreaseVolume = false;
    volatile bool songPathChanged = false;

    while (true) {
        rlock.lock(); // lock this when accessing stuff from object
        if (run==false) {
            return;
        }
        qlock.lock();
        if (!taskQueue.empty()) {

            // get the next task
            task = taskQueue.top();
            taskQueue.pop();
            qlock.unlock();

            // set a flag for the object-independent part of the loop
            switch (task) {
                case 1:
                {
                    songPathChanged = true;
                    songPath = musicDirPath;
                    songPlaying = getCurrentSongIterator();
                    songPath.append(songPlaying->second.fileName);
                    break;
                }
                case 2:
                    paused = !paused;
                    break;
                case 3:
                    increaseVolume = true;
                    break;
                case 4:
                    decreaseVolume = true;
                    break;
            }
        }
        else {
            qlock.unlock();
        }
        rlock.unlock();

        // this part of the loop accomplishes the task without talking to the object
        // this avoids undefined behavior from trying to own/access destroyed member objects/functions

        if (songPathChanged) {
            // set up to play the new song
            songPathChanged = false;
            delete buffer;
            mpg123_open(mpg123handle, songPath.c_str());
            bufferSize = mpg123_outblock(mpg123handle);
            buffer = new unsigned char[bufferSize];
            mpg123_getformat(mpg123handle, &rate, &channels, &encoding);
            format.bits = mpg123_encsize(encoding) * 8;
            format.rate = rate;
            format.channels = channels;
            format.byte_format = AO_FMT_NATIVE; // idk if big or little endian
            format.matrix = 0;
            aodevice = ao_open_live(driverID, &format, NULL);
        }

        if (increaseVolume) {
            mpg123_volume_change(mpg123handle, (double) 5);
            increaseVolume = false;
        }

        if (decreaseVolume) {
            mpg123_volume_change(mpg123handle, (double) -5);
            decreaseVolume = false;
        }

        // this is the play loop
        while (mpg123_read(mpg123handle, buffer, bufferSize, &bytesDone) == MPG123_OK) {
            if (paused) {
                 // 200ms is less time than the minimum interval at which the user is allowed to input
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
                continue;
            }
            else {
                ao_play(aodevice, (char*) buffer, bytesDone);
            }
        }

        if (paused) {
            // if it's paused, we want to redo the whole loop after sleeping, to check if unpaused
            continue;
        }
        else {
            ++songPlaying;
            songPathChanged = true;
        }
    } // end of main work loop

    // clean up before returning from worker thread
    ao_close(aodevice);
    mpg123_close(mpg123handle);
    mpg123_delete(mpg123handle);
    mpg123_exit();
    ao_shutdown();
}

