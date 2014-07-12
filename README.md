Carputer software for my Raspberry Pi
=======================================

This is a multithreaded program which turns my Raspberry Pi into a customized
car computer. It uses a character LCD and three buttons to create a drawer-like
user interface which provides mp3-playing and real-time OBD-data-reading 
functionality. I wrote it using C++, with some features (threading) from C++11.

Carputiem talks to the car through a USB cable attached to the car's OBD-II port 
and outputs sound to the audio out jack on the Pi.  

Setup notes
----------------------
Carputiem is written to work on my specific Pi setup - thus, it needs to be run
in Linux on a Raspberry Pi with buttons and a 4x20 character LCD display 
connected to the appropriate GPIO pins.

To talk to the car, the Pi needs a USB cable such as this one: 
http://www.amazon.com/ELM327-OBDII-CAN-BUS-Diagnostic-Scanner/dp/B005FEGP7I/ref=sr_1_1?s=automotive&ie=UTF8&qid=1404856981&sr=1-1

To make the magic happen when your engine turns over, plug your Pi's power 
cable into your car, and add the filepath of this program's executable to 
/etc/rc.local. The Pi will turn on when the car does, and this program will 
run with root privileges automatically at startup.

Some of this code is adapted from OBDGPSLogger, available at:
https://icculus.org/obdgpslogger/

Dependencies
----------------------
taglib.github.io
mpg123.org
https://www.xiph.org/ao/
www.wiringPi.com

Potential improvements
------------------------

1. improve the exception situation

2. add diagnostic trouble code (DTC) reading functionality





