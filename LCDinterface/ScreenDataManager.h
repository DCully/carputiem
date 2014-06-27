#ifndef SCREENDATAMANAGER_H
#define SCREENDATAMANAGER_H

#include <array>
#include <set>
#include <string>
#include "ScreenData.h"
#include "ScreenDataDrawer.h"
#include "../Exceptions.h"

class ParentNode;

class Node
{
    public:
        Node() {}
        virtual void goLeft() {}
        virtual void goRight() {}
        virtual const ScreenData& getCurrentScreenData()
            {throw invalidScrnDataMgrNodeFuncCallException;}
        ParentNode* myParent;
        bool validNode = false;
};

class LeafNode : public Node
{
    public:
        LeafNode() {}

        // for setup upon Screens' insertion to ScreenDataManager
        LeafNode(const std::vector<ScreenData> myScrns);
        const ScreenData& getCurrentScreenData();

        // for runtime use (again, only within ScreenDataManager)
        void goLeft();
        void goRight();
        bool validNode = true;
    private:
        std::vector<ScreenData> myScreens;
        int currentMyScreenIndex;
};

class ParentNode : public Node
{
    public:
        ParentNode() {}

        // for setup upon Screen's insertion to ScreenDataManager
        ParentNode(const ScreenDataDrawer& myScrnDataDrawer);
        const ScreenData& getCurrentScreenData();

        // for runtime use (only within ScreenDataManager)
        std::array<Node, 3> myLeaves;
        bool validNode = true;
    private:
        ScreenDataDrawer myScreenDataDrawer;
};

class ScreenDataManager
{
    public:
        ScreenDataManager();
        ~ScreenDataManager();

        // for Controller (button --> Controller --> ScreenDataManager.<appropriate call>())
        const ScreenData& getCurrentScreenData();

        // many Controller calls result in calling these
        void goUp();
        void goDown(int drawerNum);
        void goLeft();
        void goRight();

        // these provide the interface for ScreenData factories
        void addDrawerToDrawer(ScreenDataDrawer drawerToAdd,
                                const std::string& nameOfNewDrawerScreen,
                                const std::string& nameOfDrawerScreenToAddTo,
                                int lineOfDrawerToAddTo);
        void addScreensToDrawer(const std::vector<ScreenData>& screensToAdd,
                                const std::string& nameOfScreenToAddTo,
                                int lineOfDrawerToAddTo);
    private:
        /*
        drawers (ParentNodes) are stored in drawerMap, LeafNodes are stored in ParentNode.myLeaves
        this means there's no manual memory management... everything's in a STL container (or a ScreenData)
        drawerMap is only used when adding screens, so screen "switching" is still O(1)...
        it's just just swapping a pointer (and ultimately, reprinting)
        */
        std::map<std::string, ParentNode> drawerMap; // map of keys to drawer screens
        Node* currentNode; // keeps track of where we are in the ScreenData tree
        ParentNode root; // king of nodes
};

#endif // SCREENDATAMANAGER_H
