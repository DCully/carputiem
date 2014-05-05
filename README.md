Hello! This is a program to display a car's ECU data on a character LCD screen via the GPIO pins on a Raspberry Pi.

- This is organized around a MVC pattern; the controller relies heavily on pointers to implementation classes

- To add new functionality, just add a new ScreenData object for your new model objects to Controller::pages

- Some of this code is adapted from OBDGPSLogger, available at https://icculus.org/obdgpslogger/

- This program depends on the wiringPi library (www.wiringpi.com); this requires root privileges



Potential improvements:

0) change over to smart pointers

1) beautify cursor with a special character

2) add code to read DTCs

3) add a music player




