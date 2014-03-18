This will be a program that will run on my Raspberry Pi in my car. I'm a student, and this is a work in progress.
I'm trying to make a device that will let me view all available real time OBD2 data on a character LCD screen.

I should mention here that some of my code to interface with the OBD2 port is adapted from OBDGPSLogger,
an open source program by Gary Briggs available at https://icculus.org/obdgpslogger/.


I realize that others have done different versions of this before me. 
That's ok. 
This is my car computer. There are many like it, but this one is mine...


TO DO:

Short Term:

- IOHandler: implement update(), threaded scrolling, and new page printing

- Controller: make the factory to build ScreenData objects

- ObdSerial: convert multithreading/timer functions/etc. to C++11

Longer Term:

- add derived PID capabilities (separate class from obdserial?)

- implement SQLite database to remember cars and accompanying user settings by VIN

- add DTCs interpretation functionality

