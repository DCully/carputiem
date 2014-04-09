#include "ScreenData.h"
#include "Controller.h"
#include <iostream>

using namespace std;

extern Controller * controller;

ScreenData::ScreenData(Observable* obs, PageChangeBehavior pcb, LineSetupBehavior lsb)
    : observed(obs), pageChangeBehavior(pcb), lineSetupBehavior(lsb)
{
    // all screens have next page and previous page buttons at 17 and 19
    cursorSpots.push_back(make_pair(17, &controller->staticChangePageLeft));
    cursorSpots.push_back(make_pair(19, &controller->staticChangePageRight));

    currentSpotIndex = 0;
}

ScreenData::ScreenData() {

}

ScreenData::~ScreenData() {

}

void ScreenData::addCursorSpot(std::pair<int, SelectBehaviorFunc> newSpot) {
    if (newSpot.first >= 0 && newSpot.first < 80) {
        cursorSpots.push_back(newSpot);
    }
    else {
        cerr << "Attempted to add an invalid cursorable spot - spot out of range" << endl;
    }
}

void ScreenData::doLoadPageBehavior() {
    pageChangeBehavior.loadPage(*observed);
}

void ScreenData::doLeavePageBehavior() {
    pageChangeBehavior.leavePage(*observed);
}

LineSetupBehavior* ScreenData::getLineSetupBehavior() {
    return &lineSetupBehavior;
}

void ScreenData::moveCursorLeft(IOHandler* ioh) {
    currentSpotIndex = (currentSpotIndex+1)%cursorSpots.size();
    ioh->moveCursor(cursorSpots.at(currentSpotIndex).first);
}

void ScreenData::moveCursorRight(IOHandler* ioh) {
    currentSpotIndex = (currentSpotIndex-1)%cursorSpots.size();
    ioh->moveCursor(cursorSpots.at(currentSpotIndex).first);
}

void ScreenData::doCurSpotSelectBehavior() {
    cursorSpots.at(currentSpotIndex).second();
}

const int ScreenData::getCurrentCursorSpot() const {
    cout << "in getcurrentcursorps==spot" << endl;
    cout << "returning current cursor spot : " << cursorSpots.at(currentSpotIndex).first << endl;
    return cursorSpots.at(currentSpotIndex).first;
}











