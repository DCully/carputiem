#ifndef OBSERVER_H
#define OBSERVER_H

//"observer" interface, implemented by the view object (IOHandler)

class Observer
{
    public:
        virtual void update() = 0;
};
#endif // OBSERVER_H
