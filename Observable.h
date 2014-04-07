#ifndef OBSERVABLE_H
#define OBSERVABLE_H

#include "Observer.h"
#include <vector>
//"subject" interface, implemented by all model objects

class Observable
{
    public:
        void registerObserver(Observer * o);
        void removeObserver(Observer * o);
    protected:
        void notifyObservers(int focusPIDsIndex, std::string dataVal);
    private:
        std::vector<Observer*> observers;
};
#endif // OBSERVABLE_H
