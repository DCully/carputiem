#ifndef MUSICSCREENFACTORY_H
#define MUSICSCREENFACTORY_H

#include "MusicManager.h"
#include "../LCDinterface/MusicScreenData.h"
#include "../LCDinterface/ScreenDataManager.h"

class MusicScreenFactory
{
    public:
        MusicScreenFactory() {}
        void buildScreens(MusicManager& mm, ScreenDataManager& scdm);
};

#endif
