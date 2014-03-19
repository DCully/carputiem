#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <vector>
#include "IOHandler.h"
#include "../Observable.h"
#include <deque>
#include "ScreenData.h"

class IOHandler;

///    To add new displayable model objects:
///    1) make them inherit class Observable
///    2) choose to construct a ScreenData object or a labeledScreenData object for them
///        - both objects display each page with a title and the standard "<->" page-changing interface
///        - ScreenData objects are displayed as 3 lines of potentially scrolling text
///        - labeledScreenData objects display three lines of potentially scrolling text, with a static
///          section after each scrolling section to display a data point and a label (e.g. "2000rpm")
///    3) add the rest of your new pages's cursor-selectable spots via addCursorSpot
///        - these are function calls associated with a cursor position o screen
///    4) add your new ScreenData object to controller->pages
///    5) set up your model object to call iohandler->update(string info)
///        - "info" is the string you want the screen to refresh within the page
///        - for ScreenData objects, this is an entire line
///        - for labeledScreenData objects, it is the data point after the scrolling section of the line

class Controller
{

    friend class ScreenData;

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
        for three lines of data. Change?
        */
        ScreenData* getCurPage();
        unsigned int lastPush;
    private:
        static void changePageLeft(void);
        static void changePageRight(void);
        IOHandler * iohandler;
        std::deque<ScreenData> pages;
        ScreenData curPage;
};


#endif // CONTROLLER_H
