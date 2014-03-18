#ifndef OBSERVER_H
#define OBSERVER_H

//"observer" interface, implemented by the view object (IOHandler)
#include <string>
class Observer
{
    public:
        virtual void update(int num, std::string info) = 0;
};
#endif // OBSERVER_H
