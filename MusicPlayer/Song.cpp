#include "Song.h"

Song::Song() : fileName(""), artistName(""), albumName(""), trackNumber(0), trackName("")
{}

Song::Song(const Song& other)
:   fileName(other.fileName),
    artistName(other.artistName),
    albumName(other.albumName),
    trackNumber(other.trackNumber),
    trackName(other.trackName)
{
}

Song::Song(const std::string& filename,
           const std::string& artist,
           const std::string& album,
           const int tracknum,
           const std::string& trackname)
: fileName(filename), artistName(artist), albumName(album), trackNumber(tracknum), trackName(trackname)
{}

Song Song::operator=(const Song& other) {
    return Song(other.fileName, other.artistName, other.albumName, other.trackNumber, other.trackName);
}





