Hello! This is a program to display various data from my car on a character LCD screen via a Raspberry Pi.

I should mention here that some of my code to interface with the OBD2 port is adapted from OBDGPSLogger,
an open source program by Gary Briggs available at https://icculus.org/obdgpslogger/.
This project depends on the wiringPi library (www.wiringpi.com) and C++11.

I tried to design this project to make it easy to add different pages on the LCD screen with different information
and functionality available from different model objects without changing the view layer. To do this, you need to
build a ScreenData object for your model object and add it to the Controller's pages deque. 

To add new screens for a model object, you need to add new ScreenData objects to controller->pages.
Each ScreenData object has four important defining features:

- A LineSetupBehavior object, which encapsulates printing behavior. This class is a friend class to IOHandler, and it's
  the only one that can use IOHandler's private printing methods. 
   
- A PageChangeBehavior object, which encapsulates behavior upon arriving at or leaving the page rendered by this ScreenData
  object. This is where the ScreenData object can dictate what the model object should do when we stop/start "looking at it."
   
- A pointer to an Observable object, which is your model object. The Observable object is what the Controller will 
  register the IOHandler to observe (get update calls from) at the will of the model object. Data are pushed in a string 
  with a line number, and the LineSetupBehavior object dictates how to handle the information.
   
- Pairs of "cursorable" positions on the screen and their corresponding behavior upon selecting them. When
  the user pushes the select button, the corresponding typedef'd "SelectBehaviorFunc" function will be called. Any
  spot on the screen can be made "selectable" via "addCursorSpot". 
  


Short term goals (end of semester):

- finish the new unit tests

- clean up code (better comments, better exceptions)

- write SetupObdScreens in Controller

- finish hardware interface

- do integration testing

- make ObdSerial work better across multiple protocols


Potential additions for the future:

- derived PID capabilities (adapter class for obdserial)

- DTCs interpretation functionality (adding new features to obdserial)

- add more screens! e.g. an Mp3 player


