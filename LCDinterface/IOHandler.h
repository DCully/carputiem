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

class IOHandler
{

    public:
        IOHandler() {}
        ~IOHandler();

        IOHandler(const int& bleft, const int& bright, const int& bsel,                       //these are the three button GPIO pin numbers,
                  const int& rs, const int& strb, const int& d0, const int& d1,               //and these are the GPIO pins for the LCD (8-bit)
                  const int& d2, const int& d3, const int& d4, const int& d5,
                  const int& d6, const int& d7);

        // changes user's cursor position
        void moveCursor(const int& spot);

        // prints static text to LCD
        void printToLCD(const std::string& text, const int& spot);

        // starts up to 3 scrolling lines
        void startScrollText(const std::vector<size_t>& startSpots,
            const std::vector<size_t>& stopSpots,
            const std::vector<size_t>& lineNums,
            const std::vector<std::string>& msg);

        // flips scrolling thread to paused
        void stopAllScrollingText();

    private:
        void textScroller(); // function for the scrolling thread
        int LCDHandle; // wiringPi handle for the screen
        int cursorSpotOnScreen; // where the cursor is actually placed
        std::mutex cursor_lock; // to prevent multithreaded issues with cursor

        // these are to cleanly terminate the scrolling thread when this object is destroyed
        static volatile bool run;
        static std::mutex runLock;

        // these are to send requests to the thread
        std::mutex queueLock;
        std::queue<ScrollPacket> scrollQueue;
};

#endif // IOHANDLER_H
