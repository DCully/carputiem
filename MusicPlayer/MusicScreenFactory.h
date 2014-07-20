#ifndef MUSICSCREENFACTORY_H
#define MUSICSCREENFACTORY_H

#include "MusicManager.h"
#include "../LCDinterface/MusicScreenData.h"
#include "../LCDinterface/ScreenDataManager.h"

/*
* This class builds screens for a given MusicManager object into a ScreenDataManager object.
*/
class MusicScreenFactory
{
    public:
        MusicScreenFactory() {}
        void buildScreens(MusicManager& mm, ScreenDataManager& scdm);
};

#endif
