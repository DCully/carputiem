#ifndef OBDSERIALTEST_H
#define OBDSERIALTEST_H

#include "../ObdSerial/ObdSerial.h"
#include <cxxtest/TestSuite.h>
#include <unistd.h>

class ObdSerialTest : public CxxTest::TestSuite {

    // this is really an integration test of these methods with my car
    ObdSerial * _obd;

    public:
        void setUp() {
            _obd = new ObdSerial("/dev/ttyUSB0");
        }
        void tearDown() {
            delete _obd;
        }
        void testreadFromOBD() {
            std::string str;
            _obd->writeToOBD(12); // RPM row in obdcmds_mode1
            sleep(1);
            _obd->readFromOBD(str);
            TS_ASSERT_EQUALS(str.size(), 4); // two bytes returned
            TS_ASSERT_EQUALS(str.find_first_of(">"), std::string::npos);
            TS_ASSERT_EQUALS(str.find_first_of("X"), std::string::npos);
        }
        void testWriteToOBD() {
            TS_ASSERT_EQUALS(_obd->writeToOBD(112), -1);
        }
        void testHexStrToABCD() {
            std::string hex = "05040302";
            ObdSerial::OBDDatum datum = _obd->hexStrToABCD(hex);
            TS_ASSERT_EQUALS(datum.abcd[0], 5);
            TS_ASSERT_EQUALS(datum.abcd[1], 4);
            TS_ASSERT_EQUALS(datum.abcd[2], 3);
            TS_ASSERT_EQUALS(datum.abcd[3], 2);
            TS_ASSERT_EQUALS(datum.error, false);
           // hex = "aaaaaaaaaaaaaaaaaa";
          //  datum = _obd->hexStrToABCD(hex);
           // TS_ASSERT_EQUALS(datum.error, true);
        }
        void testFillSuppdCmds() {
            _obd->fillSuppdCmds();
            TS_ASSERT( _obd->suppdCmds.find(12) != _obd->suppdCmds.end() ); // always contains rpm
            TS_ASSERT( _obd->suppdCmds.find(4) != _obd->suppdCmds.end() ); // always contains load
        }
        void testSetFocusedPIDs() {
            _obd->fillSuppdCmds();
            std::vector<int> pids;
            pids.push_back(1111);
            pids.push_back(-2);
            _obd->setFocusedPIDs(pids);
            TS_ASSERT_EQUALS(_obd->focusPIDs.empty(), true);
            pids.push_back(4);
            pids.push_back(12);
            TS_ASSERT_EQUALS(_obd->focusPIDs.empty(), false);
        }
        void testHexToChar() {
            std::string C = "43";
            std::string G = "47";
            TS_ASSERT_EQUALS(_obd->hexToChar(C), 'C');
            TS_ASSERT_EQUALS(_obd->hexToChar(G), 'G');
        }
        void testVINCapabilities() {
            std::string VIN = _obd->getVIN();
            TS_ASSERT_LESS_THAN_EQUALS(VIN.size(), 20);
            TS_ASSERT_LESS_THAN_EQUALS(17, VIN.size());
        }
};

#endif
