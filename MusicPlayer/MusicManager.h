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

class MusicManagerInvalidKeyException : public std::exception
{
    virtual const char* what() const throw()
    {
        return "Music Manager tried to access a song from the SongMap with an invalid key";
    }
};

class MusicManagerDirReadException : public std::exception
{
    virtual const char* what() const throw()
    {
        return "Music Manager tried to read from a bad dirpath";
    }
};

/*
* This is the model object for music-playing model objects.
*/
class MusicManager : public Observable
{
    public:
        MusicManager() {}
        MusicManager(const std::string& musicDirPath);
        ~MusicManager();


        /*
        * These functions have fairly obvious jobs, right?
        */
        void togglePause();
        void increaseVolume();
        void decreaseVolume();


        /*
        *  This is how you play songs. Be sure to use the other member
        *  functions to get valid keys!
        */
        void playSong(const std::string& songKey);


        /*
        *  These functions each return a vector of pairs of songs and their keys.
        */
        std::vector<std::string> getSongKeysByArtist(const std::string& artistName);
        std::vector<std::string> getSongKeysByAlbum(const std::string& artistPlusAlbumName);


        /*
        * This returns all the artist names.
        */
        std::set<std::string> getArtistSet();

        /*
        * This one returns a pair: first == album name, second == artist+album
        */
        std::set<std::pair<std::string, std::string>> getAlbumSet();


        /*
        * This is for the Now Playing screen.
        */
        Song getCurrentSong();

        /*
        * This is for getting a song that's not currently playing (like for Song List screen).
        */
        Song getSongByKey(const std::string& key);

        /*
        * This function returns the keys of the currently browsed
        *  subset of songs. It's meant to be used by the Song List screen.
        */
        std::vector<std::string> getCurrentSongSubset();


        /*
        * This function takes an artist name or an artist+album name as input.
        *  From that, it updates what will be returned by getCurrentSongSubset.
        */
        void setCurrentSongSubset(const std::string& ArtOrArtAndAlbName);


    private:
        /*
        * these are for the current song subset implementation
        */
        std::string currentSongSubsetKey;

        /* this is the path passed into the ctor
        *
        */
        const std::string musicDirectory;

        /*
        * inserts new song into songMap, with correct key
        */
        void addSong(const std::string& songFileName);

        /*
        * reads in all ".mp3"-ending file names
        */
        std::vector<std::string> getRawMp3FileNames();

        /*
        * this is how all the Songs are stored
        * key: artistName+albumName+trackNumber+trackName
        */
        std::map<std::string, Song> songMap;

        /*
        * these are updated by each addSong call
        */
        std::set<std::string> artistSet;
        std::set<std::pair<std::string, std::string>> albumSet;

        std::mutex songLock; // needed for EVERY access to songMap
        std::string keyForCurrentSong;

        // play thread uses this to autoplay next song
        void incrementCurrentSong();

        // separate worker thread to actually play music
        void play();

        /*
        * communication with the worker thread is coordinated through the taskQueue
        * 1 - change song
        * 2 - toggle pause
        * 3 - increase volume
        * 4 - decrease volume
        * if there's nothing in the queue when it's checked, continue as before
        **/
        std::priority_queue<int> taskQueue;
        std::mutex queueLock; // needed for EVERY access to taskQueue

        // these are for killing the thread gracefully
        static std::mutex runLock;
        static volatile bool run;

        // helper function for getting key vectors
        std::vector<std::string> getSongKeysByPartialKey(const std::string& input);
};

#endif // MUSICMANAGER_H
