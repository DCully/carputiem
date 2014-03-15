This will be a program that will run on my Raspberry Pi in my car. I'm a student, and this is a work in progress.
I'm trying to make a device that will let me view all available real time OBD2 data on a character LCD screen.



Goals:

- keep the data points currently on the screen at <0.5s freshness

- plug and play with any car (so I can use it on other people's cars)



I realize that others have done different versions of this before me. 
That's ok. 
This is my car computer. There are many like it, but this one is mine...



TO DO:

1) add derived PID capabilities (separate class from obdserial?)

2) build out details of LCD/button I/O interface, adding default pages

3) add in the ability to change the page layouts, names, etc. (how?)

4) implement SQLite database to remember cars and accompanying user settings

5) add DTCs functionality
