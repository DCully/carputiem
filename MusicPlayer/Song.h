#ifndef SONG_H
#define SONG_H

#include <string>

class Song
{
    public:
        Song();
        Song(const Song& other);
        Song (const std::string& filename,
              const std::string& artist,
              const std::string& album,
              const int tracknum,
              const std::string& trackname);
        Song operator=(const Song& other);
        const std::string fileName;
        const std::string artistName;
        const std::string albumName;
        const int trackNumber;
        const std::string trackName;

};

#endif
