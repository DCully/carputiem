Hello! 

This is a program to display various data from my car on a character LCD screen via a Raspberry Pi.


    To add new screens for a model object, you need to add new ScreenData objects to controller->pages. These objects 
delegate their individual page printing behaviors and model object interaction behaviors to their LineSetupBehavior
and PageChangeBehavior objects, respectively. The view object, an IOHandler instance, is an observer, and every model
object is an Observable. 
 

    I should mention here that some of my code to interface with the OBD2 port is adapted from OBDGPSLogger,
an open source program by Gary Briggs available at https://icculus.org/obdgpslogger/.
This project depends on the wiringPi library (www.wiringpi.com) and C++11.

