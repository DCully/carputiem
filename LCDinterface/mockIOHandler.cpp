#include "mockIOHandler.h"
#include <iostream>
#include <thread>
#include "Controller.h"

// spits things out to cerr  and allows console input to sub for buttons (for testing)

using std::cin;
using std::cerr;
using std::endl;

IOHandlerInterface::IOHandlerInterface()
{
    std::thread(&IOHandlerInterface::testThread, this).detach();
}

void IOHandlerInterface::moveCursor(const int& spot)
{
    cerr << "CURSOR moved by an LSB to " << spot << endl;
}

// prints static text to LCD
void IOHandlerInterface::printToLCD(const std::string& text, const int& spot)
{
    cerr << "PRINT: '" << text << "' (" << text.size() << " chars) at " << spot << endl;
}

// starts up to 3 scrolling lines
void IOHandlerInterface::startScrollText(const std::vector<size_t>& startSpots,
                                         const std::vector<size_t>& stopSpots,
                                         const std::vector<size_t>& lineNums,
                                         const std::vector<std::string>& msg)
{
    cerr << "STARTSCROLLTEXT called: " << endl;
    for (size_t x = 0; x < lineNums.size(); ++x) {
        cerr << "  Scroll #" << x << ":" << endl;
        cerr << "    from " << startSpots.at(x) << " to " << stopSpots.at(x) << " on line " << lineNums.at(x) << endl;
        cerr << "    text: " << msg.at(x) << " (" << msg.at(x).size() << " chars) " << endl;
    }
}

// flips scrolling thread to paused
void IOHandlerInterface::stopAllScrollingText()
{
    cerr << "STOPSCROLL called" << endl;
}

void IOHandlerInterface::testThread()
{
    char input;
    while (true) {
        cin >> input;
        if (input == 'L') {
            Controller::staticLeftButPressed();
        }
        else if (input == 'R') {
            Controller::staticRightButPressed();
        }
        else if (input == 'S') {
            Controller::staticSelectPressed();
        }
    }
}
