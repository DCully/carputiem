#ifndef OBSERVER_H
#define OBSERVER_H

//"observer" interface, implemented by the view object (IOHandler)
#include <string>
#include "MusicPlayer/Song.h"

class ObserverPacket
{

};

class ObdObserverPacket: public ObserverPacket
{
    public:
        std::string information;
        int linenum;
};

class MusicObserverPacket: public ObserverPacket
{
    public:
        // contains the new currentSong
        Song currentSong;
};

class Observer
{
    public:
        virtual ~Observer() {}
        virtual void update(ObserverPacket& obsp) = 0;
};
#endif // OBSERVER_H
