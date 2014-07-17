#include "Song.h"

Song::Song() : fileName(""), artistName(""), albumName(""), trackNumber(0), trackName("")
{}


Song::Song(const std::string& filename,
           const std::string& artist,
           const std::string& album,
           const int tracknum,
           const std::string& trackname)
: fileName(filename), artistName(artist), albumName(album), trackNumber(tracknum), trackName(trackname)
{}

