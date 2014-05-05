#ifndef LINESETUPBEHAVIORS_H
#define LINESETUPBEHAVIORS_H

#include <vector>
#include <string>

class IOHandler;

class LineSetupBehavior
{
    friend class ObdFactoryTest;

    public:

        LineSetupBehavior();
        virtual ~LineSetupBehavior() {}
        LineSetupBehavior(std::vector<std::string> textForLines, const std::string& pageTitle);

        virtual LineSetupBehavior* clone() const { return new LineSetupBehavior(*this); }

        // called by controller
        virtual void renderPage(IOHandler& iohandler);

        // this does nothing (not supposed to be able to call update on the base class...
        virtual void updateLine(IOHandler* iohandler, size_t lineNum, std::string info);
    protected:
        std::string titleLine;

        std::vector<size_t> staticLineNums;
        std::vector<std::string> textForStaticLines;

        std::vector<size_t> scrollingLineNums;
        std::vector<std::string> textForScrollingLines;
};

class LabeledLineSetupBehavior: public virtual LineSetupBehavior
{
    friend class ObdFactoryTest;

    public:
        LabeledLineSetupBehavior();
        virtual ~LabeledLineSetupBehavior() {}
        LabeledLineSetupBehavior(
            std::vector<std::string> textForLines,
            std::vector<std::string> labelsForLines,
            std::vector<size_t> spaceForDataOnLines,
            const std::string& pageTitle);

        LabeledLineSetupBehavior* clone() const { return new LabeledLineSetupBehavior(*this); }

        void renderPage(IOHandler* iohandler);
        void updateLine(IOHandler* iohandler, size_t lineNum, std::string info);
    protected:
        std::vector<size_t> endSpotsForScrollingLines;
        std::vector<size_t> spaceForDataOnLines;
        std::vector<size_t> updateSpotsForLines;

};

#endif
