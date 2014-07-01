#include "ScreenDataDrawer.h"
#include "ScreenDataManager.h"

ScreenDataDrawer::ScreenDataDrawer(Observable* obs, PageChangeBehavior* pcb, DrawerLineSetupBehavior* lsb)
: ScreenData(obs,pcb,lsb)
{
    // all drawers can also be "cursored" at 39, 59, and 79
    cursorSpots.push_back(39);
    cursorSpots.push_back(59);
    cursorSpots.push_back(79);
}

