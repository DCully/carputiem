#ifndef LINESETUPBEHAVIORS_H
#define LINESETUPBEHAVIORS_H

#include <vector>
#include <string>

class IOHandler;

class LineSetupBehavior
{
    public:
        LineSetupBehavior();
        LineSetupBehavior(std::vector<std::string> textForLines, const std::string& pageTitle);
        virtual void renderLine(IOHandler* iohandler, size_t lineNum);
        virtual void updateLine(IOHandler* iohandler, size_t lineNum, std::string info);
    protected:
        std::vector<std::string> textForLines;
        std::vector<size_t> updateSpotsForLines;
        std::string title;
};

/// refactor this class
class LabeledLineSetupBehavior: public virtual LineSetupBehavior
{
    public:
        LabeledLineSetupBehavior();
        LabeledLineSetupBehavior(std::vector<std::string> textForLines,
            std::vector<std::string> labelsForLines, std::vector<size_t> spaceBtwnLblsAndTextOnLines,
            const std::string& pageTitle);
        void renderLine(IOHandler* iohandler, size_t lineNum);
        void updateLine(IOHandler* iohandler, size_t lineNum, std::string info);
    protected:
        std::vector<std::string> labelsForLines;
        std::vector<size_t> spaceBtwnLblsAndTextOnLines;
        std::vector<size_t> endOfScrollForLines;
};

#endif
