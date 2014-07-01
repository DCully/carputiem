#ifndef SCREENDATAMANAGER_H
#define SCREENDATAMANAGER_H

#include <string>
#include "ScreenData.h"
#include "ScreenDataDrawer.h"
#include "../Observer.h"
#include "../Observable.h"
#include "IOHandler.h"

class Node
{
    public:
        Node() {}
        Node(const std::string& key,
             const std::string& parKey,
             const std::vector<std::string>& children,
             const std::vector<ScreenData>& scrns);
        void goLeftInScreens();
        void goRightInScreens();
        ScreenData& getCurrentScreenData();
        const std::string myKey;
        const std::string parentKey;
        std::vector<std::string> childKeys;
    private:
        int indexOfCurrentScreen;
        std::vector<ScreenData> screens;
};

class ScreenDataManager : public Observable
{
    public:
        ScreenDataManager();
        ~ScreenDataManager() {}

        // for Controller
        ScreenData& getCurrentScreenData();
        void doCurrentSpotSelectBehavior(IOHandler& iohandler, Observer& observer);

        // these provide the interface for ScreenData factories
        void addScreens(const ScreenDataDrawer& screensToAdd,
                                    const std::string& nameOfNewDrawer,
                                    const std::string& nameOfDrawerToAddTo,
                                    int lineOfDrawerToAddTo);
        void addScreens(const std::vector<ScreenData>& screenToAdd,
                                    const std::string& nameOfNewDrawer,
                                    const std::string& nameOfDrawerToAddTo,
                                    int lineOfDrawerToAddTo);
    private:
        // for moving around in the tree
        void goUp();
        void goDownTo(int num);
        void goLeft();
        void goRight();

        std::map<std::string, Node> nodeMap; // map of keys to drawer screens
        std::string keyForCurrentNode; // keeps track of where we are in the ScreenData tree
};

#endif // SCREENDATAMANAGER_H
