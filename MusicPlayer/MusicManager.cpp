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

    // add them to the library
    for (size_t x = 0; x < songFilePaths.size(); ++x) {
        addSong(songFilePaths.at(x));
    }

    // print the library
/*
    for (auto it = songMap.begin(); it != songMap.end(); ++it) {
        std::cout << it->first << std::endl;
    }
*/

    std::unique_lock<std::mutex> slock(songLock, std::defer_lock);
    slock.lock();
    currentSongIterator = songMap.begin();
    slock.unlock();

    std::thread(&MusicManager::play, this).detach();
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
    std::unique_lock<std::mutex> locker(songLock, std::defer_lock);
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
    // sorted by ASCII values - " " is really low, and "~" is very high
    std::string lowkey = artistName;
    std::string highkey = artistName;
    while (lowkey.size() < 200) {
        lowkey.append(" ");
        highkey.append("~");
    }
    std::pair<const std::map<std::string, Song>::iterator, const std::map<std::string, Song>::iterator> myPair(songMap.lower_bound(lowkey), songMap.upper_bound(highkey));
    return myPair;
}

std::pair<const std::map<std::string, Song>::iterator, const std::map<std::string, Song>::iterator> MusicManager::getSongsByArtistFromAlbum(const std::string& artistName, const std::string& albumName)
{
    // sorted by ASCII values - " " is really low, and "~" is very high
    std::string lowkey = artistName+albumName;
    std::string highkey = artistName+albumName;
    while (lowkey.size() < 200) {
        lowkey.append(" ");
        highkey.append("~");
    }
    std::pair<const std::map<std::string, Song>::iterator, const std::map<std::string, Song>::iterator> myPair(songMap.lower_bound(lowkey), songMap.upper_bound(highkey));
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

    /// why does it have to spam std::cerr with it's invalid sample rate reports?!
    TagLib::FileRef f( (musicDirectory + songFileName).c_str() );

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

    //this makes sure track 10 doesnt preced track 1 in songMap
    std::string trackNumAsString = std::to_string(trackNumber);
    for (size_t x = 0; x < 3-trackNumAsString.size(); ++x) {
        key.append("0");
    }
    key.append(trackNumAsString);

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
    std::string lowkey = artistPlusAlbumName;
    std::string highkey = artistPlusAlbumName;

    // sorted by ASCII values - " " is really low, and "~" is very high
    while (lowkey.size() < 200) {
        lowkey.append(" ");
        highkey.append("~");
    }
    std::pair<const std::map<std::string, Song>::iterator, const std::map<std::string, Song>::iterator> myPair(songMap.lower_bound(lowkey), songMap.upper_bound(highkey));
    return myPair;
}

std::vector<std::string> MusicManager::getRawMp3FileNames() {

    dirent* directoryEntry;
    std::vector<std::string> mp3FileNames;

    DIR* directoryPtr = opendir(musicDirectory.c_str());
    if (directoryPtr==NULL) {
        throw MusicManagerDirReadException();
    }
    else {
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
void MusicManager::play() {

    std::string musicDirPath(musicDirectory);

    // for use with libraries
    long rate;
    int channels;
    int encoding;
    size_t bytesDone;
    size_t bufferSize;
    unsigned char* buffer = new unsigned char[10];
    ao_sample_format format;
    int mpgNewError;
    int driverID = ao_default_driver_id();
    mpg123_handle* mpg123handle = mpg123_new(NULL, &mpgNewError);
    ao_device* aodevice;

    // for use with thread coordination
    std::string songPath;
    std::map<std::string, Song>::iterator songPlaying;
    std::unique_lock<std::mutex> rlock(runLock, std::defer_lock);
    std::unique_lock<std::mutex> qlock(queueLock, std::defer_lock);
    std::unique_lock<std::mutex> slock(songLock, std::defer_lock);
    int task=0;

    size_t curVolumeIndex = 2;
    // seems to work ok with 1/2x^2+1, with some edge-case fiddling
    // this could definitely be improved with science
    std::vector<double> volumes;
    volumes.push_back(0.1);
    volumes.push_back(0.2);
    volumes.push_back(0.5);     // added in lower settings below (0,1)
    volumes.push_back(1);
    volumes.push_back(1.5);
    volumes.push_back(3);
    volumes.push_back(5.5);
    volumes.push_back(9);
    volumes.push_back(13.5);
    volumes.push_back(17);
    volumes.push_back(21);    // up at the top here this curve is too steep, too
    mpg123_volume(mpg123handle, volumes.at(curVolumeIndex));

    volatile bool paused = true;
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
                    paused = false;
                    songPath = musicDirPath;
                    songPlaying = getCurrentSongIterator();
                    songPath.append(songPlaying->second.fileName);
                    break;
                }
                case 2:
                    paused = !paused;
                    break;
                case 3:
                    // it's more important to turn it up
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
            buffer = new unsigned char[bufferSize+1024];
            mpg123_getformat(mpg123handle, &rate, &channels, &encoding);
            format.bits = mpg123_encsize(encoding) * 8;
            format.rate = rate;
            format.channels = channels;
            format.byte_format = AO_FMT_NATIVE; // idk if big or little endian
            format.matrix = 0;
            aodevice = ao_open_live(driverID, &format, NULL);
        }

        if (increaseVolume) {
            if (curVolumeIndex < volumes.size() - 1) {
                ++curVolumeIndex;
                mpg123_volume(mpg123handle, volumes.at(curVolumeIndex));
            }
            increaseVolume = false;
        }

        if (decreaseVolume) {
            if (curVolumeIndex > 0) {
                --curVolumeIndex;
                mpg123_volume(mpg123handle, volumes.at(curVolumeIndex));
            }
            decreaseVolume = false;
        }

        if (paused) {
            continue;
        }

        if (mpg123_read(mpg123handle, buffer, bufferSize, &bytesDone) == MPG123_OK) {
            if (paused) {
                 // 200ms is less time than the minimum interval at which the user is allowed to input
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
                continue;
            }
            else {
                ao_play(aodevice, (char*) buffer, bytesDone);
                continue;
            }
        }
        slock.lock();
        ++songPlaying;
        if (songPlaying==songMap.end()) {
            songPlaying = songMap.begin();
        }
        songPath = musicDirPath;
        songPath.append(songPlaying->second.fileName);
        songPathChanged = true;
        currentSongIterator = songPlaying;
        slock.unlock();
    } // end of main work loop

    // clean up before returning from worker thread
    ao_close(aodevice);
    mpg123_close(mpg123handle);
    mpg123_delete(mpg123handle);
    mpg123_exit();
    ao_shutdown();
}

