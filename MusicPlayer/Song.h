#ifndef SONG_H
#define SONG_H

#include <string>

/*
* Simple POD containing info for a given song file read from disk.
*/
class Song
{
    public:
        Song();
        Song (const std::string& filename,
              const std::string& artist,
              const std::string& album,
              const int tracknum,
              const std::string& trackname);
        std::string fileName;
        std::string artistName;
        std::string albumName;
        int trackNumber;
        std::string trackName;
};

#endif
