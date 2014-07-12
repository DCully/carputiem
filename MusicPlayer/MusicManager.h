#ifndef MUSICMANAGER_H
#define MUSICMANAGER_H

#include "../Observable.h"
#include <map>
#include <set>
#include <vector>
#include "mpg123.h"
#include <ao/ao.h>
#include <thread>
#include "Song.h"
#include <queue>
#include <mutex>

// this is the model object for music-playing model objects
class MusicManager : public Observable
{
    public:
        MusicManager() {}
        MusicManager(const std::string& musicDirPath);
        ~MusicManager();

        // for user's controls
        void playSong(const std::map<std::string, Song>::iterator it);
        void togglePause();
        void increaseVolume();
        void decreaseVolume();
        Song getCurrentSong();
        void setCurrentlyBrowsedSongSubset(const std::string& key);
        std::pair<const std::map<std::string, Song>::iterator, const std::map<std::string, Song>::iterator> getCurrentlyBrowsedSongSubset();

        // for MusicScreenDatas to provide content
        // these two functions return iterator pairs to the beginning and ending of the range requested
        // just increment the iterators to traverse through the songs from that artist or album
        std::pair<const std::map<std::string, Song>::iterator, const std::map<std::string, Song>::iterator> getSongsByArtist(const std::string& artistName);
        std::pair<const std::map<std::string, Song>::iterator, const std::map<std::string, Song>::iterator> getSongsByArtistFromAlbum(const std::string& artistName, const std::string& albumName);
        std::pair<const std::map<std::string, Song>::iterator, const std::map<std::string, Song>::iterator> getSongsByArtistFromAlbum(const std::string& artistPlusAlbumName);
        std::set<std::string> getArtistSet();
        std::set<std::string> getAlbumSet();
    private:
        std::string keyForCurrentlyBrowsedSongSubset;
        const std::string musicDirectory;
        void addSong(const std::string& songFileName); // inserts new song into songMap, with correct key
        std::vector<std::string> getRawMp3FileNames();
        std::map<std::string, Song> songMap; // key: artistName+albumName+trackNumber+trackName
        std::set<std::string> artistSet;
        std::set<std::string> albumSet;

        std::mutex songLock; // needed for EVERY access to currentSongIterator
        std::map<std::string, Song>::iterator currentSongIterator;
        const std::map<std::string, Song>::iterator getCurrentSongIterator();
        void setCurrentSongIterator(const std::map<std::string, Song>::iterator it);

        void play(std::string musicDirPath); // separate thread
        /*
        * communication with the worker thread is coordinated through the taskQueue
        * 1 - change song
        * 2 - toggle pause
        * 3 - increase volume
        * 4 - decrease volume
        * if there's nothing in the queue when it's checked, continue as before
        **/
        std::priority_queue<int> taskQueue;
        std::mutex queueLock;

        // these are for killing the thread gracefully
        static std::mutex runLock;
        static volatile bool run;
};

#endif // MUSICMANAGER_H
