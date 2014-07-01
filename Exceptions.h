#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

class invalidDrawerIndexException : public std::exception
{
    virtual const char* what() const throw()
    {
        return "Out of range drawer number passed to ScreenDataManager::goDown()";
    }
}invalidDrawerIndexException;

class badScreenAddException : public std::exception
{
    virtual const char* what() const throw()
    {
        return "You're trying to add screens but that name doesn't correspond to any existing drawer";
    }
}badScreenAddException;

#endif // EXCEPTIONS_H
