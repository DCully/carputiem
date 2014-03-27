#include "ScreenData.h"
#include "Controller.h"
#include <iostream>

using namespace std;

extern Controller * controller;

ScreenData::ScreenData(Observable* obs, PageChangeBehavior pcb, LineSetupBehavior lsb, string titl)
    : pageChangeBehavior(pcb), lineSetupBehavior(lsb), observed(obs), title(titl.substr(0,15))
{
    // all screens have next page and previous page buttons at 17 and 19
    currentSpot = make_pair(17, &controller->changePageLeft);
    cursorSpots.push_front(make_pair(19, &controller->changePageRight));
}

ScreenData::ScreenData() {

}

ScreenData::~ScreenData() {

}

void ScreenData::addCursorSpot(std::pair<int, SelectBehaviorFunc> newSpot) {
    cursorSpots.push_back(newSpot);
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




