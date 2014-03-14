This will be a program that will run on my Raspberry Pi in my car. I'm a student, and this is a work in progress.


Project Goals: View real time OBD2 data through a character LCD screen based interface

- keep the data points currently on the screen at <0.5s freshness

- easily flip through pages, customize pages, read DTC codes, view VIN, etc

- calculate a number of derived PIDs (e.g. real time gas mileage)

- store settings for each car in an SQLite database upon shutdown

- plug and play with any car (so I can use it on other people's cars)



I realize that others have done different versions of this before me. 
That's ok. 
This is my car computer. There are many like it, but this one is mine...



TO DO:

- improve/add exception handling

- change ObdSerial to access three PIDs at a time (specified via a setFocusedPIDs() function)

- after each PID read, have ObdSerial notify its observers, passing the PID/datum pair

- have the controller spawn ObdSerial and IOHandler, and use getVIN to determine if the car has a DB entry

- write the changed settings to the DB upon exit, to remember cars

- build out details of LCD/button I/O interface

- put const timeout variables, etc into a config.h file


Whew. That's enough for a while.
