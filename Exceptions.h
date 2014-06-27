#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H


class badNodeCastException : public std::exception
{
    virtual const char* what() const throw()
    {
        return "Bad Node Pointer Cast in ScreenDataManager::goDown()";
    }
}badNodeCastException;

class invalidDrawerIndexException : public std::exception
{
    virtual const char* what() const throw()
    {
        return "Out of range drawer number passed to ScreenDataManager::goDown()";
    }
}invalidDrawerIndexException;

class invalidParentNodeAccessException : public std::exception
{
    virtual const char* what() const throw()
    {
        return "ScreenDataManager::goDown() called when currentNode has NULL parent";
    }
}invalidParentNodeAccessException;

class badScreenAddException : public std::exception
{
    virtual const char* what() const throw()
    {
        return "You're trying to add screens but that name doesn't correspond to any existing drawer";
    }
}badScreenAddException;

class invalidScrnDataMgrNodeFuncCallException : public std::exception
{
    virtual const char* what() const throw()
    {
        return "Somehow you called a virtual function on the base class Node - dont do that";
    }
}invalidScrnDataMgrNodeFuncCallException;
#endif // EXCEPTIONS_H
