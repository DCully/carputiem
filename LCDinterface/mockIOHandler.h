#ifndef MOCKIOHANDLER_H
#define MOCKIOHANDLER_H

#include <string>
#include <vector>

/* This is the generalized IOHandler interface
*  All these virtual functions are overriden by the IOHandler class
*  This parent class prints everything to console instead of a character LCD
*  This way, we can test everything but new IOHandler implementations without a Pi
*  Attach keyboard inputs to simulate GPIO button presses?
*/
class IOHandlerInterface
{
    public:
        IOHandlerInterface();
        virtual ~IOHandlerInterface() {}

        // changes user's cursor position
        virtual void moveCursor(const int& spot);

        // prints static text to LCD
        virtual void printToLCD(const std::string& text, const int& spot);

        // starts up to 3 scrolling lines
        virtual void startScrollText(const std::vector<size_t>& startSpots,
            const std::vector<size_t>& stopSpots,
            const std::vector<size_t>& lineNums,
            const std::vector<std::string>& msg);

        // flips scrolling thread to paused
        virtual void stopAllScrollingText();
    private:
        void testThread();
};

#endif
