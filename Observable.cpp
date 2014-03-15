#include <vector>
#include "Observer.h"
#include "Observable.h"
#include <algorithm>

void Observable::registerObserver(Observer * o) {
    observers.push_back(o);
}

void Observable::removeObserver(Observer * o) {
    observers.erase(std::remove(observers.begin(), observers.end(), o), observers.end());
}

void Observable::notifyObservers(int focusPIDsIndex, double dataVal) {
    std::vector<Observer*>::iterator it;
    for(it = observers.begin(); it != observers.end(); ++it) {
        //*it.update();
    }
}
