#ifndef IOHANDLERTEST_H
#define IOHANDLERTEST_H

#include "../LCDinterface/Controller.h"
#include "../LCDinterface/IOHandler.h"
#include "wPi_mock.h"
#include <cxxtest/TestSuite.h>

class MockLCD : 
public T::Base_lcdInit, public T::Base_lcdCursor, public T::Base_lcdPosition, public T::Base_lcdPuts, public T::Base_lcdCursorBlink, 
public T::Base_wiringPiSetup, public T::Base_wiringPiISR
{

    public:
    
        int lcdInit(int rows, int cols, int bits, int rs, int strb, int d0, int d1, int d2, int d3, int d4, int d5, int d6, int d7) {
            return 1;
        }
        
        void lcdPosition(int handle, int x, int y) {

        }
        
        void lcdCursor(int fd, int state) {

        }
        
        void lcdPuts(int handle, const char *string) {

        }
        
        void lcdCursorBlink(int fd, int state) {

        }
        
        int wiringPiSetup(void) {
            return 1;
        }
        
        int wiringPiISR(int pin, int edgeType,  void(*function)(void) ) {
            return 1;
        }
        
};

class IOHandlerTest : public CxxTest::TestSuite {

    IOHandler* _ioh;
    Controller* _cont;
    public:
    
        void setUp() {
            MockLCD t();
            _ioh = new IOHandler(1,1,1,1,1,1,1,1,1,1,1,1,1, _cont);
        }
        
        void tearDown() {
            delete _ioh;
        }

        void testMoveCursor() {

        }
        
        void testStartScrollText() {
        
        }
        
        void testStopScrollTextOnLine() {
        
        }
        
        void testPrintToLCD() {
        
        }
        
};

#endif
