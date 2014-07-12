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

class badSerialConnectException : public std::exception
{
    virtual const char* what() const throw()
    {
        return "Serial port failed to connect in ObdSerial";
    }
}badSerialConnectException;

class MusicManagerDirReadException : public std::exception
{
    virtual const char* what() const throw()
    {
        return "Music Manager tried to read from a bad dirpath";
    }
}MusicManagerDirReadException;

class IOHandlerSetupException : public std::exception
{
    virtual const char* what() const throw()
    {
        return "Display could not be set up properly through GPIO pins";
    }
}IOHandlerSetupException;


#endif // EXCEPTIONS_H
