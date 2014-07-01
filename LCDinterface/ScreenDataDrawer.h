#ifndef SCREENDATADRAWER_H
#define SCREENDATADRAWER_H

#include "LineSetupBehaviors.h"
#include "ScreenData.h"

class ScreenDataDrawer : public ScreenData
{
    public:
        ScreenDataDrawer() {}
        ScreenDataDrawer(Observable* obs, PageChangeBehavior* pcb, DrawerLineSetupBehavior* lsb);
};

#endif // SCREENDATADRAWER_H
