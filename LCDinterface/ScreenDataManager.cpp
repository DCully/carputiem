#include "ScreenDataManager.h"
#include <iostream>
#include "../Exceptions.h"

/// TODO:
/// 1) ctor, addDrawerToDrawer, addScreensToDrawer
/// 2)



using std::cerr;
using std::endl;

/// LeafNode

LeafNode::LeafNode(const std::vector<ScreenData> myScrns) {
    myScreens = myScrns;
    myParent = NULL;
}

void LeafNode::goLeft() {
    currentMyScreenIndex = (currentMyScreenIndex-1)%myScreens.size();
}

void LeafNode::goRight() {
    currentMyScreenIndex = (currentMyScreenIndex+1)%myScreens.size();
}

const ScreenData& LeafNode::getCurrentScreenData() {
    return myScreens.at(currentMyScreenIndex);
}



/// ParentNode

const ScreenData& ParentNode::getCurrentScreenData() {
    return myScreenDataDrawer;
}

ParentNode::ParentNode(const ScreenDataDrawer& myScrnDataDrawer) {
    myScreenDataDrawer = myScrnDataDrawer;
    myParent = NULL;
}



/// ScreenDataManager

ScreenDataManager::ScreenDataManager()
{
/*
    ParentNode root
    drawerMap.insert(std::pair<std::string, ParentNode>("root", ))

    // etc
*/
}

const ScreenData& ScreenDataManager::getCurrentScreenData() {
    return currentNode->getCurrentScreenData();
}

// changes currentNode
void ScreenDataManager::goUp() {
    if (currentNode->myParent==NULL) {
        cerr << "Error in ScreenDataManager::goUp() - parentNode marked as null" << endl;
        throw invalidParentNodeAccessException;
    }
    else {
        currentNode = currentNode->myParent; // go back up the tree
    }
}

// changes currentNode
void ScreenDataManager::goDown(int drawerNum) {

    if (drawerNum < 0 || drawerNum > 3) {
        cerr << "Out of range drawerNum passed to ScreenDataManager::goDown()" << endl;
        throw invalidDrawerIndexException;
    }

    ParentNode* curParentNode = dynamic_cast<ParentNode*>(currentNode);

    if (curParentNode == NULL) {
        cerr << "Error casting currentNode to ParentNode* in ScreenDataManager::goDown()" << endl;
        cerr << "This is probably because it tried to cast a different Node type to a ParentNode" << endl;
        throw badNodeCastException;
    }

    // this tests if the Node is of class Node (as when myLeaves was built) or if it's been replaced
    // by either ParentNode or LeafNode - functional, but messy...
    if (curParentNode->myLeaves.at(drawerNum).validNode==false) {
        cerr << "Error in ScreenDataManager::goDown() - tried to go down through an invalid drawer in currentNode" << endl;
        throw invalidDrawerIndexException;
    }

    currentNode = &curParentNode->myLeaves.at(drawerNum); // change currentNode to point to the newly focused leaf
}

// preserves currentNode
void ScreenDataManager::goLeft() {
    currentNode->goLeft();
}

// preserves currentNode
void ScreenDataManager::goRight(){
    currentNode->goRight();
}

// for adding drawer screens
void ScreenDataManager::addDrawerToDrawer(ScreenDataDrawer drawerToAdd,
                                        const std::string& nameForNewDrawerScreen,
                                        const std::string& nameOfDrawerScreenToAddTo,
                                        int lineOfDrawerToAddTo)
{
    if (drawerMap.count(nameOfDrawerScreenToAddTo) == 0) {
        cerr << "You tried to add a drawer to a drawer whose name didn't represent any drawer" << endl;
        throw badScreenAddException;
    }

    if (drawerMap.count(nameForNewDrawerScreen)==1) {
        cerr << "You tried to add a new drawer, but the name was taken" << endl;
        throw badScreenAddException;
    }

    if (lineOfDrawerToAddTo < 1 || lineOfDrawerToAddTo > 3) {
        cerr << "You tried to add a drawer with an invalid line number - use 1, 2, or 3" << endl;
        throw invalidDrawerIndexException;
    }

    // build ParentNode for drawerToAdd

    // add it to drawerMap

    // set its parent to nameOfDrawerScreenToAddTo

    // add new ParentNode to drawer at lineOfDrawerToAddTo in nameOfScreenToAddTo
}

// for adding content screens
void ScreenDataManager::addScreensToDrawer(const std::vector<ScreenData>& screenToAdd,
                                        const std::string& nameOfDrawerScreenToAddTo,
                                        int lineOfDrawerToAddTo)
{
    if (drawerMap.count(nameOfDrawerScreenToAddTo) == 0) {
        cerr << "You tried to add some screens to a drawer whose name didn't represent any drawer" << endl;
        throw badScreenAddException;
    }

    if (lineOfDrawerToAddTo < 1 || lineOfDrawerToAddTo > 3) {
        cerr << "You tried to add a drawer with an invalid line number - use 1, 2, or 3" << endl;
        throw invalidDrawerIndexException;
    }

    // build LeafNode for screenToAdd

    // set its parent to nameOfDrawerScreenToAddTo

    // add new LeafNode to drawer at lineOfDrawerToAddTo in nameOfScreenToAddTo
}
