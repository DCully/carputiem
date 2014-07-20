#ifndef SCROLLPACKET_H
#define SCROLLPACKET_H

#include <vector>
#include <string>

/*
* Plain old data bundle for pushing into IOHandler's thread queue.
*/
class ScrollPacket
{
    public:
        ScrollPacket();
        ScrollPacket(bool sleep);
        ScrollPacket(std::vector<size_t> startSpot,
                     std::vector<size_t> stopSpot,
                     std::vector<size_t> lineNum,
                     std::vector<std::string> msg);
        bool sleep;
        std::vector<size_t> startSpots;
        std::vector<size_t> stopSpots;
        std::vector<size_t> lineNums;
        std::vector<std::string> msgsForLines;
};

#endif // SCROLLPACKET_H
