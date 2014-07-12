#include "ScreenData.h"
#include "Controller.h"
#include <iostream>

ScreenData::ScreenData(Observable* obs, PageChangeBehavior* pcb, LineSetupBehavior* lsb)
    : observed(obs), pageChangeBehavior(pcb), lineSetupBehavior(lsb)
{
    // all screens have next page and previous page buttons at 17 and 19
    cursorSpots.push_back(17);
    cursorSpots.push_back(18);
    cursorSpots.push_back(19);

    currentSpotIndex = 0;
}

ScreenData::ScreenData() {

}

ScreenData::~ScreenData() {
    delete pageChangeBehavior;
    delete lineSetupBehavior;
    // pages don't own their model objects
}

int ScreenData::getCurrentCursorSpot() {
    return cursorSpots.at(currentSpotIndex);
}

ScreenData::ScreenData(const ScreenData& other) :
    observed(other.observed),
    pageChangeBehavior(other.pageChangeBehavior->clone()),
    lineSetupBehavior(other.lineSetupBehavior->clone()),
    cursorSpots(other.cursorSpots),
    currentSpotIndex(other.currentSpotIndex)
{ }

ScreenData& ScreenData::operator=(ScreenData other) {
    ScreenData temp(other);
    swap(temp);
    return *this;
}

void ScreenData::swap(ScreenData& other) {
    std::swap(observed, other.observed);
    std::swap(lineSetupBehavior, other.lineSetupBehavior);
    std::swap(pageChangeBehavior, other.pageChangeBehavior);
    std::swap(currentSpotIndex, other.currentSpotIndex);
    std::swap(cursorSpots, other.cursorSpots);
}

void ScreenData::doLoadPageBehavior() {
    currentSpotIndex = 0;
    pageChangeBehavior->loadPage(*observed);
}

void ScreenData::doLeavePageBehavior() {
    pageChangeBehavior->leavePage(*observed);
}

void ScreenData::printPage(IOHandler& ioh) {
    lineSetupBehavior->renderPage(ioh);
}

LineSetupBehavior* ScreenData::getLineSetupBehavior() {
    return lineSetupBehavior;
}

void ScreenData::moveCursorLeft(IOHandler& ioh) {
    currentSpotIndex = (currentSpotIndex+1)%cursorSpots.size();
    ioh.moveCursor(cursorSpots.at(currentSpotIndex));
}

void ScreenData::moveCursorRight(IOHandler& ioh) {
    currentSpotIndex = (currentSpotIndex-1)%cursorSpots.size();
    ioh.moveCursor(cursorSpots.at(currentSpotIndex));
}

void ScreenData::addCursorSpot(int spot) {
    if (spot >= 0 && spot < 80) {
        cursorSpots.push_back(spot);
    }
    else {
        std::cerr << "Attempted to add an invalid cursorable spot - spot out of range" << std::endl;
    }
}

// override this to add new cursor-able spots to a screen (not 17, 18, 19, 39, 59, or 79)
void ScreenData::doCurSpotSelectBehavior() {
    /// ADD MODEL OBJECT INTERACTIONS HERE
}


