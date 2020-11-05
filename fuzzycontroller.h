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
    void runFuzzyController(float distFront, float distLeft, float distRight);

    double getOutputDirection(){ return double(dir->getValue()); }
    double getOutputVelocity(){ return double(vel->getValue()); }

private:

    double outputDirection = 0;
    float outputVelocity = 0;

    Engine* engine = new Engine;

    InputVariable* disF = new InputVariable;
    InputVariable* disL = new InputVariable;
    InputVariable* disR = new InputVariable;

    OutputVariable* vel = new OutputVariable;
    OutputVariable* dir = new OutputVariable;

    RuleBlock* mamdani = new RuleBlock;


};

#endif // FUZZYCONTROLLER_H
