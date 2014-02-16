#ifndef OBSERVABLE_H
#define OBSERVABLE_H

#include <vector>
//"subject" interface, implemented by all model objects

class Observable
{
    public:
        Observable();
        void registerObserver(Observer * o);
        void removeObserver(Observer * o);
    protected:
        void notifyObservers(void);
    private:
        std::vector<Observer*> observers;
};
#endif // OBSERVABLE_H
