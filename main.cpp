#include "./ObdSerial/ObdSerial.h"
#include "./LCDinterface/IOHandler.h"
#include "./LCDinterface/Controller.h"
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <vector>
#include <set>
#include <string>

Controller * controller;

int main(void)
{
/*
    MusicManager* mm = new MusicManager("/home/david/Music/from linnett/");

    std::set<std::string> artists = mm->getArtistSet();
    std::set<std::string> albums = mm->getAlbumSet();

    // print the artist and album lists
    std::cerr<<"ARTISTS:"<<std::endl;
    for (auto it = artists.begin(); it != artists.end(); ++it) {
        std::cerr << "    " << *it << std::endl;
    }
    std::cerr << std::endl << "ALBUMS:"<< std::endl;
    for (auto it = albums.begin(); it != albums.end(); ++it) {
        std::cerr << "    " << *it << std::endl;
    }
    std::cerr<< std::endl << std::endl;

    auto it = artists.begin();
    ++it;
    ++it;
    ++it;

    std::pair<const std::map<std::string, Song>::iterator, const std::map<std::string, Song>::iterator> song = mm->getSongsByArtistFromAlbum(*it);
    std::map<std::string, Song>::iterator firstSong = song.first;
    mm->playSong(firstSong);
    for (int x = 0; x < 6; ++x) {
        sleep(2);
        std::cerr<<"Current Song: " << mm->getCurrentSong().trackName << std::endl;
        mm->increaseVolume();
    }
    for (int x = 0; x < 6; ++x) {
        sleep(2);
        std::cerr<<"Current Song: " << mm->getCurrentSong().trackName << std::endl;
        mm->decreaseVolume();
    }
*/
    return 0;
}

