#include <wiringPi.h>
#include <lcd.h>
#include "IOHandler.h"
#include <string>
#include "Observer.h"
#include "ScreenData.h"
#include <cstdlib>
#include <sstream>
#include <iostream>

using namespace std;

//view layer

IOHandler::IOHandler(int bleft, int bright, int bsel,               //these are the three button GPIO pin numbers
                  int rs, int strb, int d0, int d1,                 //and these are the GPIO pins for the LCD, in 8-bits
                  int d2, int d3, int d4, int d5, int d6, int d7, Controller * cont)
{
    wiringPiSetup();
    LCDHandle = lcdInit(4,20,8, rs,strb, d0,d1,d2,d3,d4,d5,d6,d7);
    wiringPiISR(bleft, INT_EDGE_FALLING, &Controller::lButPressed);
    wiringPiISR(bright, INT_EDGE_FALLING, &Controller::rButPressed);
    wiringPiISR(bsel, INT_EDGE_FALLING, &Controller::selPressed);

    lcdCursor(LCDHandle, 1);
    lcdCursorBlink(LCDHandle, 1);
    cursorPosition = 18;
    moveCursor(cursorPosition);
    controller = cont;
}

void IOHandler::moveCursor(int spot) {
   cursorPosition = spot;
   lcdPosition(LCDHandle, spot%20, spot/20);
}

int IOHandler::getCurPos() {
    return cursorPosition;
}

void IOHandler::printScreen(string text) {
    lcdPosition(LCDHandle, 0,0);
    lcdPuts(LCDHandle, text.c_str());
    lcdPosition(LCDHandle, cursorPosition%20, cursorPosition/20);
}

void IOHandler::update() {
	ScreenData* data = controller->getCurPageInfo();
    string output = "";
    string pNum = "";
    string dt = "";
    ostringstream s;

    //build first line
    if (data->title.size() >= 15) {
        data->title = data->title.substr(0,15);
        output += data->title;
    }
    else {
        output += data->title;
        for (unsigned int x = 0; x < (15-data->title.size()); x++) {
            output += " ";
        }
    }
    output += "  ";
    output += "<"; // spot 17
    output += "-"; // spot 18
    output += ">"; // spot 19
    //end of first line
	s.str("");
	s.clear();

    //build other lines
    for (int x = 0; x < 3; x++) {
        if (data->dataPage == false) {
            /*this is the escape for non-data pages
            if the screendata struct's dataPage flag is false,
            IOHandler just prints the nameArray strings
            */
            if (data->nameArray[x].size() > 20) {
                output += data->nameArray[x].substr(0,20);
            }
            else {
                output += data->nameArray[x];
                for (unsigned int x = 0; x < ( 20-(data->nameArray[x].size()) ); x++) {
                    output += " ";
                }
            }
        }
        else { // so, if it IS a data page...
            //uses extra space not used by name, label, and data line, to pad middle

            //reformat the name first - range up to 10
            if (data->nameArray[x].size() >= 9) {
                data->nameArray[x] = data->nameArray[x].substr(0,9);
            }

            //then do the label - range up to 3
			if (data->lblArray[x].size() > 3) {
				data->lblArray[x] = data->lblArray[x].substr(0,3);
			}
			//TODO: change data processing operation so that it accounts for sig figs
			s << data->dataArray[x];
			dt = s.str();
            dt = dt.substr(0, 6);
            while ( data->lblArray[x].size() + data->nameArray[x].size() + dt.size() < 20 ) {
                data->nameArray[x] += " ";
            }
            output += data->nameArray[x];
			output += dt;
			output += data->lblArray[x];
			s.str("");
			s.clear();
            dt = "";
        }
    } //end of for loop
    printScreen(output);
    delete data;

}//end of update()
