#ifndef CONTROLLERTEST_H
#define CONTROLLERTEST_H

#include "../LCDinterface/Controller.h"
#include "../LCDinterface/ScreenData.h"
#include "../LCDinterface/LineSetupBehaviors.h"
#include "../LCDinterface/PageChangeBehaviors.h"
#include <cxxtest/TestSuite.h>

class ControllerTest : public CxxTest::TestSuite {

    public:
    
        Controller * _cont;
        ScreenData * _sd;
        LineSetupBehavior * _lsb;
        PageChangeBehavior * _pcb;
        ScreenData * _sd2;
        LineSetupBehavior * _lsb2;
        PageChangeBehavior * _pcb2;
        Observable * _obs;
    
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

            std::vector<std::string> testTextForLines2;
            
            std::string one2 = "hello";
            std::string two2 = "test";
            std::string three2 = "hummushummushummus";
            
            testTextForLines2.push_back(one2);
            testTextForLines2.push_back(two2);
            testTextForLines2.push_back(three2);
            
            _lsb2 = new LineSetupBehavior(testTextForLines2);
            
            _obs2 = new Observable();
            
            _pcb2 = new PageChangeBehavior(*_obs);
            
            std::string t2 = "Different Title! chopped off";
            
            _sd2 = new ScreenData(_obs2, *_pcb2, *_lsb2, t2);
            
            _cont->pages.push_back(_sd2);

        }       
        
        void tearDown() {
            delete _sd;
            delete _lsb;
            delete _pcb;
            delete _sd2;
            delete _lsb2;
            delete _pcb2;
            delete _cont;
        }
    
        void testChangePageLeft() {
            _cont->changePageLeft();
            std::string t2 = "Different Title";
            TS_ASSERT_EQUALS(_cont->getCurPage().title, t2);
            _cont->changePageLeft();
            std::string t1 = "Test title";
            TS_ASSERT_EQUALS()_cont->getCurPage().title, t1);
        }

        void testChangePageRight() {
            _cont->changePageLeft();
            std::string t2 = "Different Title";
            TS_ASSERT_EQUALS(_cont->getCurPage().title, t2);
            _cont->changePageLeft();
            std::string t1 = "Test title";
            TS_ASSERT_EQUALS()_cont->getCurPage().title, t1);        
        }

};

#endif
