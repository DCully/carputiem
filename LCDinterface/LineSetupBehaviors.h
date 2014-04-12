#ifndef LINESETUPBEHAVIORS_H
#define LINESETUPBEHAVIORS_H

#include <vector>
#include <string>

class IOHandler;

class LineSetupBehavior
{
    public:
        LineSetupBehavior();
        virtual ~LineSetupBehavior() {}
        LineSetupBehavior(std::vector<std::string> textForLines, const std::string& pageTitle);
        virtual void renderLine(IOHandler* iohandler, size_t lineNum);
        virtual void updateLine(IOHandler* iohandler, size_t lineNum, std::string info);
        virtual LineSetupBehavior* clone() const { return new LineSetupBehavior(*this); }
    protected:
        std::vector<std::string> textForLines;
        std::vector<size_t> updateSpotsForLines; // these are left justified internally
        std::string title;
};

/// refactor this class
class LabeledLineSetupBehavior: public virtual LineSetupBehavior
{
    public:
        LabeledLineSetupBehavior();
        virtual ~LabeledLineSetupBehavior() {}
        LabeledLineSetupBehavior(
            std::vector<std::string> textForLines,
            std::vector<std::string> labelsForLines,
            std::vector<size_t> spaceForDataOnLines,
            const std::string& pageTitle);
        void renderLine(IOHandler* iohandler, size_t lineNum);
        void updateLine(IOHandler* iohandler, size_t lineNum, std::string info);
        LabeledLineSetupBehavior* clone() const { return new LabeledLineSetupBehavior(*this); }
    protected:
        std::vector<std::string> labelsForLines;
        std::vector<size_t> spacesForDataOnLine;
        std::vector<size_t> endOfScrollsForLine; // these get right justified internally
};

#endif
