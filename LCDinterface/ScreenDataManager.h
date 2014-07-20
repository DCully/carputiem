#ifndef SCREENDATAMANAGER_H
#define SCREENDATAMANAGER_H

#include <string>
#include "ScreenData.h"
#include "ScreenDataDrawer.h"
#include "../Observer.h"
#include "../Observable.h"
#include "mockIOHandler.h"
#include <stack>
#include <iostream>

class invalidDrawerIndexException : public std::exception
{
    virtual const char* what() const throw()
    {
        return "Out of range drawer number passed to ScreenDataManager::goDown()";
    }
};

class badScreenAddException : public std::exception
{
    virtual const char* what() const throw()
    {
        return "You're trying to add screens but that name doesn't correspond to any existing drawer";
    }
};

/*
* Nodes own their ScreenData objects (on the heap)
* since I subclass ScreenData a lot, and the details are determined
* at runtime, this is necessary, in order to avoid object slicing
* of the ScreenData objects in the STL containers
*/
class Node
{
    public:
        Node() {}
        ~Node();
        Node(const std::string key,
             const std::string parKey,
             const std::vector<std::string> children,
             const std::vector<ScreenData*> scrns);
        Node& operator=(Node other);
        Node(const Node& other);
        void swap(Node& other);
        void goLeftInScreens();
        void goRightInScreens();
        ScreenData& getCurrentScreenData();

        std::string myKey;
        std::string parentKey;
        std::vector<std::string> childKeys;
    private:
        int indexOfCurrentScreen;
        std::vector<ScreenData*> screens;
};

/*
* This class is owned by the Controller and maintains a graph of ScreenData objects
* which represent the interconnected screens of the user interface.
* The various addScreen functions are for adding screens during setup - after that,
* This class basically exists to manage Select button presses and return the current
* ScreenData object when asked to.
*/
class ScreenDataManager : public Observable
{
    public:
        ScreenDataManager();
        ~ScreenDataManager() {}

        // for Controller
        ScreenData& getCurrentScreenData();
        void doCurrentSpotSelectBehavior(IOHandlerInterface& iohandler, Observer& observer);

        // these provide the interface for ScreenData factories
        void addScreens(ScreenDataDrawer* screensToAdd,
                                    const std::string& nameOfNewDrawer,
                                    const std::string& nameOfDrawerToAddTo,
                                    int lineOfDrawerToAddTo);
        // this is to add a screen with just one parent
        void addScreens(const std::vector<ScreenData*> screenToAdd,
                                    const std::string& nameOfNewDrawer,
                                    const std::string& nameOfDrawerToAddTo,
                                    int lineOfDrawerToAddTo);
        // this is to add a screen with a bunch of parent drawers
        void addScreens(const std::vector<ScreenData*> screenToAdd,
                                    const std::string& nameForNewScreens,
                                    const std::vector<std::pair<std::string, int>>& parentDrawers);
    private:
        // for moving around in the tree
        void goUp();
        void goDownTo(int num);
        void goLeft();
        void goRight();

        std::stack<std::string> backStack;
        std::map<std::string, Node> nodeMap; // map of keys to drawer screens
        std::string keyForCurrentNode; // keeps track of where we are in the ScreenData tree
};

#endif // SCREENDATAMANAGER_H
