
#include "IOHandler.h"
#include "Controller.h"
using namespace std;

Controller * controller; //this is the global controller object created by thread main in order handle interrupts

int main()
{
    controller = new Controller();
    //test
    //controller->iohandler->update();
    //use this to test buttons (should print to cout)
    while (true) {}
    return 0;
}

