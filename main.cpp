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

using namespace std;

int main(void)
{

    controller = new Controller();
    while (true) {}

/*
    MusicManager* mm = new MusicManager("/home/david/Music/from linnett/");

    std::set<std::string> artistKeys = mm->getArtistSet();

    mm->setCurrentSongSubset(*artistKeys.begin());

    std::vector<std::string> songs = mm->getCurrentSongSubset();

    mm->playSong(songs.at(0));

    sleep(5);
    mm->togglePause();
    sleep(5);
    mm->togglePause();

    cout << mm->getCurrentSong().trackName << endl;


    while (true) {}
*/
    return 0;
}

