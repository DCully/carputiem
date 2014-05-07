Carputiem - Carputer software for my Raspberry Pi
=======================================

Hello! This is a program to display a car's ECU data from a Raspberry Pi. It creates a simple interface for user input via two buttons that move a cursor around on a screen, and a third which "selects" the currently "cursored" position. The screen displays data on a number of "pages" which the user flips through by select-buttoning arrows on the LCD screen. It can also scroll text on the screen when necessary using some simple multithreading.

The program talks to the car through a USB cable attached to the OBD-II port (usually somewhere under the driver's side dashboard). Once the connection is established, it figures out which data points the car is capable of providing, and then generates pages to show each datum, along with names and units. Because OBD-II is federally standardized, this program should run on any OBD-II enabled vehicle (most cars made after 1996). 

Setup notes
----------------------
To be of use, this code needs to be run in Linux on a Raspberry Pi with a 4x20 character LCD display. However, it needs to interface with a couple of pieces of hardware in a certain way. The LCD is wired in 8-bit mode, as described here:
http://wiringpi.com/dev-lib/lcd-library/

The left, right, and select buttons need to be wired to wiringPi pin numbers 8, 9, and 12. I wired the circuit using the same design as shown here:
https://projects.drogon.net/raspberry-pi/gpio-examples/tux-crossing/3-more-leds-and-a-button/

To talk to the car, the Pi needs a USB cable such as this one: 
http://www.amazon.com/ELM327-OBDII-CAN-BUS-Diagnostic-Scanner/dp/B005FEGP7I/ref=sr_1_9?ie=UTF8&qid=1399432070&sr=8-9&keywords=elm327

My code (in Controller.cpp) assumes this cable is at ttyUSB0 - however, if yours shows up at a different address, it's a pretty simple change to make.


Code notes
----------------------

- This is organized around a MVC pattern; the controller relies heavily on pointers to implementation classes

- To add new functionality, just add a new ScreenData object for your new model object(s) to the controller's "pages" vector

- Some of this code is adapted from OBDGPSLogger, available at https://icculus.org/obdgpslogger/

- This program depends on the wiringPi library (www.wiringpi.com), which requires root privileges



Potential improvements:

0) change over to smart pointers

1) beautify cursor with a special character

2) add code to read DTCs

3) add a music player




