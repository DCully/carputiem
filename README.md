Carputer software for my Raspberry Pi
=======================================

This is a multithreaded program which turns my Raspberry Pi into a customized
car computer. It uses a character LCD and three buttons to create a user 
interface, and it plays mp3 files and reads data from my ECU in real time. 

The code is organized in an MVC design pattern.

View
----------------------
The "view" functionalities are encapsulated in the IOHandler class, which I 
mocked out in order to test without the Pi. This relies on the wiringPi library 
to communicate with the character LCD and GPIO pins.

Controller
---------------------
The "controller" layer (class Controller) recieves inputs from the View layer but
delegates much of its functionalities to class ScreenDataManager, which handles
which "ScreenData" object (which screen) the user is currently viewing. 

These ScreenData objects, in turn, group together the details of where the cursor 
is and what a given screen is supposed to do. It contains the individual implementations
for selecting any given cursor-able position on its screen.

Model
--------------------
The model basically consists of two sections. 

The first is the ObdSerial class, which manages communication to the car 
via a serial port. 

The second is the MusicManager class, which reads in the songs and their info
from a given directory and provides the interface for playing songs, pausing 
songs, and finding your way through the library. 

Notes
----------------------
Carputiem is written to work on my specific Pi setup, so it needs to be run
in Linux on a Raspberry Pi with buttons and a 4x20 character LCD display 
connected to the appropriate GPIO pins.

To talk to the car, the I used a USB cable not unlike this one: 
http://www.amazon.com/ELM327-OBDII-CAN-BUS-Diagnostic-Scanner/dp/B005FEGP7I/ref=sr_1_1?s=automotive&ie=UTF8&qid=1404856981&sr=1-1

Some of this code (a table of OBD functions) is adapted from OBDGPSLogger:
https://icculus.org/obdgpslogger/

Dependencies
----------------------
taglib.github.io
mpg123.org
https://www.xiph.org/ao/
www.wiringPi.com

Potential improvements
---------------------
1) More extensive exception handling/error code detection with libao and libmpg123

2) Supplementary features - add DTC reading to ObdSerial, new file formats to MusicManager




