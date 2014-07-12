#include "ScrollPacket.h"

ScrollPacket::ScrollPacket()
{
    //ctor
}

ScrollPacket::ScrollPacket(bool slp)
{
    sleep = slp;

}

ScrollPacket::ScrollPacket(std::vector<size_t> startSpot,
                           std::vector<size_t> stopSpot,
                           std::vector<size_t> lineNum,
                           std::vector<std::string> msg)
: startSpots(startSpot), stopSpots(stopSpot), lineNums(lineNum), msgsForLines(msg)
{
    sleep = false;
}
