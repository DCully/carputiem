#include "ScreenDataManager.h"
#include <iostream>
#include "../Exceptions.h"

using std::cerr;
using std::endl;

Node::Node(const std::string& key,
    const std::string& parKey,
    const std::vector<std::string>& children,
    const std::vector<ScreenData*>& scrns  ) : myKey(key),
                                              parentKey(parKey),
                                              childKeys(children),
                                              screens(scrns)
{
    indexOfCurrentScreen = 0;
}

Node::~Node() {
    for (size_t x = 0; x < screens.size(); ++x) {
        delete screens.at(x);
    }
}

void Node::goLeftInScreens() {
    indexOfCurrentScreen = (indexOfCurrentScreen-1)%screens.size();
}

void Node::goRightInScreens() {
    indexOfCurrentScreen = (indexOfCurrentScreen+1)%screens.size();
}

ScreenData& Node::getCurrentScreenData() {
    return *screens.at(indexOfCurrentScreen);
}



/// ScreenDataManager

ScreenDataManager::ScreenDataManager() {

    // build vector with home drawer in it
    PageChangeBehavior* pcb = new PageChangeBehavior;
    std::vector<std::string> drawerNames;
    drawerNames.push_back("Music");
    drawerNames.push_back("Vehicle Data");
    drawerNames.push_back("Settings");
    DrawerLineSetupBehavior* dlsb = new DrawerLineSetupBehavior(drawerNames, "Home");
    ScreenDataDrawer* homeDrawer = new ScreenDataDrawer(this, pcb, dlsb);
    std::vector<ScreenData*> homeScreen(1, homeDrawer);

    // build home node
    std::vector<std::string> children(3, " ");
    Node homeNode("home", NULL, children, homeScreen);

    // add home node to map
    nodeMap.insert(std::pair<std::string, Node>("home", homeNode));
    backStack.push("home");
}

ScreenData& ScreenDataManager::getCurrentScreenData() {
    return nodeMap.at(keyForCurrentNode).getCurrentScreenData();
}

void ScreenDataManager::goUp() {
    keyForCurrentNode = backStack.top();
    backStack.pop();
}

void ScreenDataManager::goDownTo(int num) {
    keyForCurrentNode = nodeMap.at(keyForCurrentNode).childKeys.at(num-1);
    backStack.push(keyForCurrentNode);
}

void ScreenDataManager::goLeft() {
    nodeMap.at(keyForCurrentNode).goLeftInScreens();
}

void ScreenDataManager::goRight() {
    nodeMap.at(keyForCurrentNode).goRightInScreens();
}

void ScreenDataManager::addScreens(const std::vector<ScreenData*>& screensToAdd,
                                    const std::string& nameForNewScreens,
                                    const std::string& nameOfDrawerToAddTo,
                                    int lineOfDrawerToAddTo)
{
    if ( lineOfDrawerToAddTo < 1 || lineOfDrawerToAddTo > 3 ) {
        cerr << "You tried to add new screens to an invalid drawer number" << endl;
        throw invalidDrawerIndexException;
    }
    if (nodeMap.count(nameForNewScreens)==1) {
        cerr << "You tried to add new screens with an already-taken name" << endl;
        throw badScreenAddException;
    }
    if (nodeMap.count(nameForNewScreens)==0) {
        cerr << "You tried to add new screens to a non-existent drawer" << endl;
        throw badScreenAddException;
    }

    // build the new Node and add it to the nodeMap
    std::vector<std::string> newVector(3, " ");
    Node newNode(nameForNewScreens, nameOfDrawerToAddTo, newVector, screensToAdd);
    std::pair<std::string, Node> newPair(nameForNewScreens, newNode);
    nodeMap.insert(newPair);

    // update the parent node
    nodeMap.at(nameOfDrawerToAddTo).childKeys.at(lineOfDrawerToAddTo-1) = nameForNewScreens;
}

void ScreenDataManager::addScreens(ScreenDataDrawer* screenToAdd,
                                    const std::string& nameForNewScreens,
                                    const std::string& nameOfDrawerToAddTo,
                                    int lineOfDrawerToAddTo)
{
    if ( lineOfDrawerToAddTo < 1 || lineOfDrawerToAddTo > 3 ) {
        cerr << "You tried to add new screens to an invalid drawer number" << endl;
        throw invalidDrawerIndexException;
    }
    if (nodeMap.count(nameForNewScreens)==1) {
        cerr << "You tried to add new screens with an already-taken name" << endl;
        throw badScreenAddException;
    }
    if (nodeMap.count(nameForNewScreens)==0) {
        cerr << "You tried to add new screens to a non-existent drawer" << endl;
        throw badScreenAddException;
    }

    // build the new Node and add it to the nodeMap
    std::vector<std::string> newVector(3, " ");
    std::vector<ScreenData*> screensToAdd(1, screenToAdd); // small vector for drawer
    Node newNode(nameForNewScreens, nameOfDrawerToAddTo, newVector, screensToAdd);
    std::pair<std::string, Node> newPair(nameForNewScreens, newNode);
    nodeMap.insert(newPair);

    // update the parent node
    nodeMap.at(nameOfDrawerToAddTo).childKeys.at(lineOfDrawerToAddTo-1) = nameForNewScreens;
}

void ScreenDataManager::doCurrentSpotSelectBehavior(IOHandler& iohandler, Observer& observer) {
    int spot = getCurrentScreenData().getCurrentCursorSpot();
    switch (spot) {
        case 17:
        {
            // get a pointer to save lookups
            Node* currentNode = &nodeMap.at(keyForCurrentNode);

            // unhook page from observed
            currentNode->getCurrentScreenData().doLeavePageBehavior();
            currentNode->getCurrentScreenData().observed->removeObserver(&observer);

            // change page
            currentNode->goLeftInScreens();

            // hook up new page to observed
            currentNode->getCurrentScreenData().doLoadPageBehavior();
            currentNode->getCurrentScreenData().observed->registerObserver(&observer);

            // print the new page
            currentNode->getCurrentScreenData().printPage(iohandler);
            break;
        }
        case 18:
        {
            // get a pointer to save lookups
            Node* currentNode = &nodeMap.at(keyForCurrentNode);

            // unhook page from observed
            currentNode->getCurrentScreenData().doLeavePageBehavior();
            currentNode->getCurrentScreenData().observed->removeObserver(&observer);

            // change page
            goUp();

            // update the pointer
            currentNode = &nodeMap.at(keyForCurrentNode);

            // hook up new page to observed
            currentNode->getCurrentScreenData().doLoadPageBehavior();
            currentNode->getCurrentScreenData().observed->registerObserver(&observer);

            // print the new page
            currentNode->getCurrentScreenData().printPage(iohandler);
            break;
        }
        case 19:
        {
            // get a pointer to save lookups
            Node* currentNode = &nodeMap.at(keyForCurrentNode);

            // unhook page from observed
            currentNode->getCurrentScreenData().doLeavePageBehavior();
            currentNode->getCurrentScreenData().observed->removeObserver(&observer);

            // change page
            currentNode->goRightInScreens();

            // hook up new page to observed
            currentNode->getCurrentScreenData().doLoadPageBehavior();
            currentNode->getCurrentScreenData().observed->registerObserver(&observer);

            // print the new page
            currentNode->getCurrentScreenData().printPage(iohandler);
            break;
        }
        case 39:
        {
            // get a pointer to save lookups
            Node* currentNode = &nodeMap.at(keyForCurrentNode);

            // unhook page from observed
            currentNode->getCurrentScreenData().doLeavePageBehavior();
            currentNode->getCurrentScreenData().observed->removeObserver(&observer);

            // change page
            goDownTo(1);

            // update the pointer
            currentNode = &nodeMap.at(keyForCurrentNode);

            // hook up new page to observed
            currentNode->getCurrentScreenData().doLoadPageBehavior();
            currentNode->getCurrentScreenData().observed->registerObserver(&observer);

            // print the new page
            currentNode->getCurrentScreenData().printPage(iohandler);
            break;
        }
        case 59:
        {
            // get a pointer to save lookups
            Node* currentNode = &nodeMap.at(keyForCurrentNode);

            // unhook page from observed
            currentNode->getCurrentScreenData().doLeavePageBehavior();
            currentNode->getCurrentScreenData().observed->removeObserver(&observer);

            // change page
            goDownTo(2);

            // update the pointer
            currentNode = &nodeMap.at(keyForCurrentNode);

            // hook up new page to observed
            currentNode->getCurrentScreenData().doLoadPageBehavior();
            currentNode->getCurrentScreenData().observed->registerObserver(&observer);

            // print the new page
            currentNode->getCurrentScreenData().printPage(iohandler);
            break;
        }
        case 79:
        {
            // get a pointer to save lookups
            Node* currentNode = &nodeMap.at(keyForCurrentNode);

            // unhook page from observed
            currentNode->getCurrentScreenData().doLeavePageBehavior();
            currentNode->getCurrentScreenData().observed->removeObserver(&observer);

            // change page
            goDownTo(3);

            // update the pointer
            currentNode = &nodeMap.at(keyForCurrentNode);

            // hook up new page to observed
            currentNode->getCurrentScreenData().doLoadPageBehavior();
            currentNode->getCurrentScreenData().observed->registerObserver(&observer);

            // print the new page
            currentNode->getCurrentScreenData().printPage(iohandler);
            break;
        }
        default:
            // otherwise we're not navigating - we're doing stuff with content of ScreenData
            getCurrentScreenData().doCurSpotSelectBehavior();
    }
}




