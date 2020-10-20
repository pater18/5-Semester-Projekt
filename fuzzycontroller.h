#ifndef FUZZYCONTROLLER_H
#define FUZZYCONTROLLER_H

#include "fl/Headers.h"
#include "lidarsensor.h"

using namespace fl;


class fuzzyController
{
public:
    fuzzyController();

    void setupFuzzyController();
    void runFuzzyController();

    double getOutputDirection(){ return mSteer->getValue(); }

private:

    double outputDirection;

    Engine* engine = new Engine;

    InputVariable* obstacle = new InputVariable;
    OutputVariable* mSteer = new OutputVariable;
    RuleBlock* mamdani = new RuleBlock;


};

#endif // FUZZYCONTROLLER_H
