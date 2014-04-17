#ifndef OBDFACTORY_H
#define OBDFACTORY_H

#include "../LCDinterface/ScreenData.h"
#include "../ObdSerial/ObdSerial.h"

class ObdFactory
{
    public:
        ObdFactory(ObdSerial* obs);
        virtual ~ObdFactory();
        // takes a vector of PID indices from obdcmds_mode1 as input
        // outputs ScreenData objects into the ScreenData vector
        void buildObdScreens(const std::vector<int>& obdIndices, std::vector<ScreenData>& pages);
    protected:
    private:
        ObdSerial* obds;
};

#endif // OBDFACTORY_H
