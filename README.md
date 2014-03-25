This will be a program that will run on my Raspberry Pi in my car. I'm a student, and this is a work in progress.
I'm trying to make a device that will let me view all available real time OBD2 data on a character LCD screen.

I should mention here that some of my code to interface with the OBD2 port is adapted from OBDGPSLogger,
an open source program by Gary Briggs available at https://icculus.org/obdgpslogger/.


TO DO:

Short Term:

- write implementation LineSetupBehavior classes and rewrite iohandler's update() function

- write tests and debug

- edit code for better readability

Longer Term Additions (end of April):

- derived PID capabilities (separate class from obdserial?)

- DTCs interpretation functionality (again, separate model object?)

- more screen pages, with different functionalities (e.g. Mp3 player)


