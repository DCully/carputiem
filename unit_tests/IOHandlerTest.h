#ifndef IOHANDLERTEST_H
#define IOHANDLERTEST_H

#include "../LCDinterface/IOHandler.h"
#include "../LCDinterface/Controller.h"
#include "../LCDinterface/ScreenData.h"
#include <cxxtest/TestSuite.h>

class IOHandlerTest : public CxxTest::TestSuite {

    // this needs to be connected to the LCD to pass tests 
    // also needs the serial port opening in Controller's constructor commented out
    // to get around this, you'd need to mock up the wiringPi library and the serial port interface

    Controller * _cont;
    // _cont->iohandler
    
    ScreenData * _sd;
    LineSetupBehavior * _lsb;
    PageChangeBehavior * _pcb;
    Observable * _obs;
    

    public:
    
        void setUp() {
            _cont = new Controller();
            
            std::vector<std::string> testTextForLines;
            
            std::string one = "this one's short";
            std::string two = "another line";
            std::string three = "";
            
            testTextForLines.push_back(one);
            testTextForLines.push_back(two);
            testTextForLines.push_back(three);
            
            _lsb = new LineSetupBehavior(testTextForLines);
            
            _obs = new Observable();
            
            _pcb = new PageChangeBehavior(*_obs);
            
            std::string t = "Test title";
            
            _sd = new ScreenData(_obs, *_pcb, *_lsb, t);
            
            _cont->curPage = *_sd;
        }
        
        void tearDown() {
            delete _sd;
            delete _lsb;
            delete _pcb;
            delete _cont;
        }

        void testPrintPage() {
            /// pass in _cont->curPage and then test that printToLCD rendered it properly
            _cont-<iohandler->printPage(_cont->curPage);
        }
        
        void testUpdate() {
            /// print the same page as before and then call update and verify that it changes lines properly
            
        }

        void testMoveCursor() {
            _cont->iohandler->moveCursor(15);
            TS_ASSERT_EQUALS(_cont->iohandler->cursorPosition, 15);
            TS_ASSERT_THROWS_ANYTHING(_cont->iohandler->moveCursor(99));
        }


};

#endif
