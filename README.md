
This will be a program that will run on my Raspberry Pi in my car. I'm a student, and this is a work in progress.
I'm trying to make a device that will let me view all available real time OBD2 data on a character LCD screen.

I should mention here that some of my code to interface with the OBD2 port is adapted from OBDGPSLogger,
an open source program by Gary Briggs available at https://icculus.org/obdgpslogger/.

This project depends on the wiringPi library (www.wiringpi.com). Other than that, all it needs is C++11.

I tried to design this project to make it easy to add different pages on the LCD screen with different information
and functionality available from different model objects without changing the view layer. To do this, you need to
build a ScreenData object for your model object and add it to the Controller's pages deque. 

The design of the ScreenData object is what makes it easy to add new model objects. Each one has a few important things:

1) A LineSetupBehavior object, which encapsulates printing behavior. This class is a friend class to IOHandler, and it's
   the only one that can use IOHandler's private printing methods. 
   
2) A PageChangeBehavior object, which encapsulates behavior upon arriving at or leaving the page rendered by this ScreenData
   object. This is where the ScreenData object can dictate what the model object should do when we stop/start "looking at it."
   
3) A pointer to an Observable object, which is your model object. The Observable object is what the Controller will 
   register the IOHandler to observe (get update calls from) at the will of the model object. Data are pushed in a string 
   with a line number, and the LineSetupBehavior object dictates how to handle the information.
   
4) A deque of pairs of "cursorable" positions on the screen and their corresponding behavior upon selecting them. When
   the user pushes the select button, the corresponding typedef'd "SelectBehaviorFunc" function will be called. All 
   ScreenData objects are automatically given behavior for spots 17 and 19 on the screen, because these are the page 
   changing arrows. However, any other spot on the screen can be made "selectable" as well by adding an appropriately
   constructed cursor-position to SelectBehaviorFunc pair via ScreenData function "addCursorSpot". 
   
That's all it takes to extend LCD interface functionality to a new model object. It's basically an application of the
Observer patterns and Strategy patterns within an MVC-styled program. I think it's nifty.


TO DO:

Short Term (end of March):

- add exceptions everywhere

- write LineSetupBehaviorsTest.h, ScreenDataTest.h, IOHandlerTest.h 

- finish hardware interface


Longer Term Additions (end of April):

- derived PID capabilities (separate class from obdserial?)

- DTCs interpretation functionality (again, separate model object?)

- more screen pages, with different functionalities (e.g. Mp3 player)


