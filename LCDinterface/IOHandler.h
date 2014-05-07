#ifndef IOHANDLER_H
#define IOHANDLER_H

#include <string>
#include "../Observer.h"
#include <mutex>
#include "ScreenData.h"
#include <thread>
#include <vector>

class IOHandler
{

    public:
        IOHandler() {}
        virtual ~IOHandler() {}

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

        // flips scrolling off and waits for the thread to rejoin
        void stopAllScrollingText();

    private:
        void textScroller(std::vector<size_t> startSpot,
            std::vector<size_t> stopSpot,
            std::vector<size_t> lineNum,
            std::vector<std::string> msg);
        int LCDHandle;
        int cursorSpotOnScreen;
        std::thread ScrollingThread;
        volatile bool TextIsScrolling;
        std::mutex cursor_lock;
};

#endif // IOHANDLER_H
