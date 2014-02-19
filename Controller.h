#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <vector>
#include "IOHandler.h"
#include "ScreenData.h"

class IOHandler;

class Controller
{
    public:
        Controller();
        /*These three functions handle button input calls from
        the iohandler. They do different things depending on where
        the cursor is (as passed into the function) and what page
        the view layer is on.
        */
        static void lButPressed();
        static void rButPressed();
        static void selPressed();
        /*
        This function is called by the view's update function.
        It returns a struct of the page number, title, and info
        for three lines of data. There's no chance of page and
        data lines going out of sync because the function uses
        the page number directly in order to pull the data out
        of the three vectors.
        */
        ScreenData* getInfoAddresses();
        int getCurPos();
        unsigned int lastPush;
    protected:
        /*this just sets the page for now, but later will also
        be used to change the view's observable registration.
        By doing both in this function (and this function only!),
        we can ensure that they never go out of sync, later on.
        */
        void setViewPage(unsigned int input);
    private:
        //I'm still not sure how to store references to data that's in model objects...
        std::vector<double*> dataVals;
        std::vector<std::string*> dataLbls;
        std::vector<std::string*> dataUnits;
        IOHandler * iohandler;
        unsigned int viewPage;
};
#endif // CONTROLLER_H
