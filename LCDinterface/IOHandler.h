#ifndef IOHANDLER_H
#define IOHANDLER_H

#include <string>
#include "../Observer.h"
#include <mutex>
#include "ScreenData.h"
#include <thread>
#include <vector>
#include <queue>
#include "ScrollPacket.h"
#include "mockIOHandler.h" // this is the generalized IOHandler interface

class IOHandlerSetupException : public std::exception
{
    virtual const char* what() const throw()
    {
        return "Display could not be set up properly through GPIO pins";
    }
};

/*
*  This is the class that encapsulates printing to the LCD and reading inputs
*  from the left, right, and select buttons. Its parent class mocks its interface, for testing.
*/
class IOHandler: public IOHandlerInterface
{

    public:
        IOHandler() {}
        ~IOHandler();

        /*
        * The first three inputs are the wiringPi GPIO pin numbers for the
        * left, right, and select button interrupts, respectively.
        * The remaining inputs (d0 - d7) are the pin numbers for the LCD.
        */
        IOHandler(const int& bleft, const int& bright, const int& bsel,                       //these are the three button GPIO pin numbers,
                  const int& rs, const int& strb, const int& d0, const int& d1,               //and these are the GPIO pins for the LCD (8-bit)
                  const int& d2, const int& d3, const int& d4, const int& d5,
                  const int& d6, const int& d7);

        /*
        * Changes user's cursor position.
        * PrintToLCD calls this independently of the user - as such,
        * the cursor is locked by a mutex.
        */
        void moveCursor(const int& spot);

        /*
        *  Prints static text to LCD at spot%80.
        */
        void printToLCD(const std::string& text, const int& spot);

        /*
        *  This starts up to 3 scrolling lines.
        *  Stop other scrolling things first!
        */
        void startScrollText(const std::vector<size_t>& startSpots,
            const std::vector<size_t>& stopSpots,
            const std::vector<size_t>& lineNums,
            const std::vector<std::string>& msg);

        /*
        * This flips scrolling thread to paused (not scrolling anything).
        */
        void stopAllScrollingText();

    private:
        void textScroller(); // function for the scrolling thread
        int LCDHandle; // wiringPi handle for the screen
        int cursorSpotOnScreen; // where the cursor is actually placed
        std::mutex cursor_lock; // to prevent multithreaded issues with cursor

        // these are to cleanly terminate the scrolling thread when this object is destroyed
        volatile bool run;
        std::mutex runLock;
        std::thread myThread;

        // these are to send requests to the thread
        std::mutex queueLock;
        std::queue<ScrollPacket> scrollQueue;
};

#endif // IOHANDLER_H
