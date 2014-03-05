#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <vector>
#include "IOHandler.h"
#include "ScreenData.h"
#include <deque>

class IOHandler;

class Controller
{
    public:
        Controller();
        ~Controller();
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
        for three lines of data.
        */
        ScreenData* getCurPageInfo();
        unsigned int lastPush;
    protected:
        void changePage(bool left);
    private:
        std::vector<double*> dataVals;
        std::vector<std::string*> dataLbls;
        std::vector<std::string*> dataUnits;
        IOHandler * iohandler;
        unsigned int viewPage;
        std::deque<ScreenData> pages;
        ScreenData curPage;
};
#endif // CONTROLLER_H
