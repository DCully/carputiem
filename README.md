This will be a program that will run on my Raspberry Pi in my car. I'm a student, and this is a work in progress.
I'm trying to make a device that will let me view all available real time OBD2 data on a character LCD screen.

I should mention here that some of my code to interface with the OBD2 port is adapted from OBDGPSLogger,
an open source program by Gary Briggs available at https://icculus.org/obdgpslogger/.


TO DO:

Short Term:

- IOHandler: implement update()

- Controller: make the factory to build ScreenData objects

- Write tests. Lots of tests.

Longer Term:

- add derived PID capabilities (separate class from obdserial?)

- implement SQLite database to remember cars and other user settings by VIN

- add DTCs interpretation functionality

- more screen pages, with different functionalities (like Mp3 playing)


