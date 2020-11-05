#include "fuzzycontroller.h"

fuzzyController::fuzzyController()
{

}


void fuzzyController::setupFuzzyController(){

    // Fuzzy Logic Controller
    engine->setName("ObstacleAvoidance");
    engine->setDescription("");


    // Inputs to the controller

    // Front distance input
    disF->setName("disF");
    disF->setDescription("");
    disF->setEnabled(true);
    disF->setRange(0.000, 10.000);
    disF->setLockValueInRange(false);
    disF->addTerm(new Ramp("close", 1.500, 1.000));
    disF->addTerm(new Triangle("medium", 1.000, 1.500, 2.000));
    disF->addTerm(new Ramp("okay", 1.500, 2.000));
    engine->addInputVariable(disF);

    // Left distance input
    disL->setName("disL");
    disL->setDescription("");
    disL->setEnabled(true);
    disL->setRange(0.000, 10.000);
    disL->setLockValueInRange(false);
    disL->addTerm(new Ramp("close", 1.000, 0.500));
    disL->addTerm(new Triangle("medium", 0.500, 1.000, 1.500));
    disL->addTerm(new Ramp("okay", 1.000, 1.500));
    engine->addInputVariable(disL);

    // Right distance input
    disR->setName("disR");
    disR->setDescription("");
    disR->setEnabled(true);
    disR->setRange(0.000, 10.000);
    disR->setLockValueInRange(false);
    disR->addTerm(new Ramp("close", 1.000, 0.500));
    disR->addTerm(new Triangle("medium", 0.500, 1.000, 1.500));
    disR->addTerm(new Ramp("okay", 1.000, 1.500));
    engine->addInputVariable(disR);


    // Outputs from the controller

    // Direction
    dir->setName("dir");
    dir->setDescription("");
    dir->setEnabled(true);
    dir->setRange(-0.400, 0.400);
    dir->setLockValueInRange(false);
    dir->setAggregation(new Maximum);
    dir->setDefuzzifier(new Centroid(100));
    dir->setDefaultValue(fl::nan);
    dir->setLockPreviousValue(false);
    dir->addTerm(new Ramp("fastL", -0.200, -0.400));
    dir->addTerm(new Triangle("slowL", -0.400, -0.200, 0.000));
    dir->addTerm(new Triangle("none", -0.200, 0.000, 0.200));
    dir->addTerm(new Triangle("slowR", 0.000, 0.200, 0.400));
    dir->addTerm(new Ramp("fastR", 0.200, 0.400));
    engine->addOutputVariable(dir);

    // Velocity
    vel->setName("vel");
    vel->setDescription("");
    vel->setEnabled(true);
    vel->setRange(0, 1.200);
    vel->setLockValueInRange(false);
    vel->setAggregation(new Maximum);
    vel->setDefuzzifier(new Centroid(100));
    vel->setDefaultValue(fl::nan);
    vel->setLockPreviousValue(false);
    vel->addTerm(new Ramp("slow", 0.300, 0.100)); // change values
    vel->addTerm(new Triangle("medium", 0.100, 0.300, 0.500));
    vel->addTerm(new Ramp("fast", 0.300, 0.500));
    engine->addOutputVariable(vel);


    // Rule Block/Base for the controller
    mamdani->setName("mamdani");
    mamdani->setDescription("");
    mamdani->setEnabled(true);
    mamdani->setConjunction(new AlgebraicProduct);
    mamdani->setDisjunction(new Maximum);
    mamdani->setImplication(new AlgebraicProduct);
    mamdani->setActivation(new General);

//    // Front distance okay
//    mamdani->addRule(Rule::parse("if disF is okay and disL is okay and disR is okay then dir is none and vel is fast", engine));
//    mamdani->addRule(Rule::parse("if disF is okay and disL is okay and disR is medium then dir is none and vel is fast", engine));
//    mamdani->addRule(Rule::parse("if disF is okay and disL is okay and disR is close then dir is slowL and vel is medium", engine));
//    mamdani->addRule(Rule::parse("if disF is okay and disL is medium and disR is okay then dir is none and vel is fast", engine));
//    mamdani->addRule(Rule::parse("if disF is okay and disL is medium and disR is medium then dir is none and vel is fast", engine));
//    mamdani->addRule(Rule::parse("if disF is okay and disL is medium and disR is close then dir is slowL and vel is medium", engine));
//    mamdani->addRule(Rule::parse("if disF is okay and disL is close and disR is okay then dir is fastR and vel is medium", engine));
//    mamdani->addRule(Rule::parse("if disF is okay and disL is close and disR is medium then dir is slowR and vel is medium", engine));
//    mamdani->addRule(Rule::parse("if disF is okay and disL is close and disR is close then dir is none and vel is medium", engine));

//    // Front distance medium
//    mamdani->addRule(Rule::parse("if disF is medium and disL is okay and disR is okay then dir is slowL and vel is medium", engine)); // slowL/R
//    mamdani->addRule(Rule::parse("if disF is medium and disL is okay and disR is medium then dir is slowL and vel is medium", engine));
//    mamdani->addRule(Rule::parse("if disF is medium and disL is okay and disR is close then dir is fastL and vel is medium", engine));
//    mamdani->addRule(Rule::parse("if disF is medium and disL is medium and disR is okay then dir is slowR and vel is medium", engine));
//    mamdani->addRule(Rule::parse("if disF is medium and disL is medium and disR is medium then dir is slowL and vel is medium", engine)); //slowL/R
//    mamdani->addRule(Rule::parse("if disF is medium and disL is medium and disR is close then dir is slowL and vel is medium", engine));
//    mamdani->addRule(Rule::parse("if disF is medium and disL is close and disR is okay then dir is fastR and vel is medium", engine));
//    mamdani->addRule(Rule::parse("if disF is medium and disL is close and disR is medium then dir is slowR and vel is medium", engine));
//    mamdani->addRule(Rule::parse("if disF is medium and disL is close and disR is close then dir is none and vel is slow", engine));

//    // Front distance close
//    mamdani->addRule(Rule::parse("if disF is close and disL is okay and disR is okay then dir is fastL and vel is slow", engine)); // fastL/R
//    mamdani->addRule(Rule::parse("if disF is close and disL is okay and disR is medium then dir is fastL and vel is slow", engine));
//    mamdani->addRule(Rule::parse("if disF is close and disL is okay and disR is close then dir is fastL and vel is slow", engine));
//    mamdani->addRule(Rule::parse("if disF is close and disL is medium and disR is okay then dir is fastR and vel is slow", engine));
//    mamdani->addRule(Rule::parse("if disF is close and disL is medium and disR is medium then dir is fastL and vel is slow", engine)); // fastL/R
//    mamdani->addRule(Rule::parse("if disF is close and disL is medium and disR is close then dir is fastL and vel is slow", engine));
//    mamdani->addRule(Rule::parse("if disF is close and disL is close and disR is okay then dir is fastR and vel is slow", engine));
//    mamdani->addRule(Rule::parse("if disF is close and disL is close and disR is medium then dir is fastR and vel is slow", engine));
//    mamdani->addRule(Rule::parse("if disF is close and disL is close and disR is close then dir is slowL and vel is slow", engine)); // Maybe turn around - stuck situation


    // Front distance okay
    mamdani->addRule(Rule::parse("if disF is okay and disL is okay and disR is okay then dir is none", engine));
    mamdani->addRule(Rule::parse("if disF is okay and disL is okay and disR is medium then dir is none", engine));
    mamdani->addRule(Rule::parse("if disF is okay and disL is okay and disR is close then dir is slowL", engine));
    mamdani->addRule(Rule::parse("if disF is okay and disL is medium and disR is okay then dir is none", engine));
    mamdani->addRule(Rule::parse("if disF is okay and disL is medium and disR is medium then dir is none", engine));
    mamdani->addRule(Rule::parse("if disF is okay and disL is medium and disR is close then dir is slowL", engine));
    mamdani->addRule(Rule::parse("if disF is okay and disL is close and disR is okay then dir is fastR", engine));
    mamdani->addRule(Rule::parse("if disF is okay and disL is close and disR is medium then dir is slowR", engine));
    mamdani->addRule(Rule::parse("if disF is okay and disL is close and disR is close then dir is none", engine));

    // Front distance medium
    mamdani->addRule(Rule::parse("if disF is medium and disL is okay and disR is okay then dir is slowL", engine)); // slowL/R
    mamdani->addRule(Rule::parse("if disF is medium and disL is okay and disR is medium then dir is slowL", engine));
    mamdani->addRule(Rule::parse("if disF is medium and disL is okay and disR is close then dir is fastL", engine));
    mamdani->addRule(Rule::parse("if disF is medium and disL is medium and disR is okay then dir is slowR", engine));
    mamdani->addRule(Rule::parse("if disF is medium and disL is medium and disR is medium then dir is slowL", engine)); //slowL/R
    mamdani->addRule(Rule::parse("if disF is medium and disL is medium and disR is close then dir is slowL", engine));
    mamdani->addRule(Rule::parse("if disF is medium and disL is close and disR is okay then dir is fastR", engine));
    mamdani->addRule(Rule::parse("if disF is medium and disL is close and disR is medium then dir is slowR", engine));
    mamdani->addRule(Rule::parse("if disF is medium and disL is close and disR is close then dir is none", engine));

    // Front distance close
    mamdani->addRule(Rule::parse("if disF is close and disL is okay and disR is okay then dir is fastL", engine)); // fastL/R
    mamdani->addRule(Rule::parse("if disF is close and disL is okay and disR is medium then dir is fastL", engine));
    mamdani->addRule(Rule::parse("if disF is close and disL is okay and disR is close then dir is fastL", engine));
    mamdani->addRule(Rule::parse("if disF is close and disL is medium and disR is okay then dir is fastR", engine));
    mamdani->addRule(Rule::parse("if disF is close and disL is medium and disR is medium then dir is fastL", engine)); // fastL/R
    mamdani->addRule(Rule::parse("if disF is close and disL is medium and disR is close then dir is fastL", engine));
    mamdani->addRule(Rule::parse("if disF is close and disL is close and disR is okay then dir is fastR", engine));
    mamdani->addRule(Rule::parse("if disF is close and disL is close and disR is medium then dir is fastR", engine));
    mamdani->addRule(Rule::parse("if disF is close and disL is close and disR is close then dir is slowL", engine)); // Maybe turn around - stuck situation


    // Front distance okay
    mamdani->addRule(Rule::parse("if disF is okay and disL is okay and disR is okay then vel is fast", engine));
    mamdani->addRule(Rule::parse("if disF is okay and disL is okay and disR is medium then vel is fast", engine));
    mamdani->addRule(Rule::parse("if disF is okay and disL is okay and disR is close then vel is medium", engine));
    mamdani->addRule(Rule::parse("if disF is okay and disL is medium and disR is okay then vel is fast", engine));
    mamdani->addRule(Rule::parse("if disF is okay and disL is medium and disR is medium then vel is fast", engine));
    mamdani->addRule(Rule::parse("if disF is okay and disL is medium and disR is close then vel is medium", engine));
    mamdani->addRule(Rule::parse("if disF is okay and disL is close and disR is okay then vel is medium", engine));
    mamdani->addRule(Rule::parse("if disF is okay and disL is close and disR is medium then vel is medium", engine));
    mamdani->addRule(Rule::parse("if disF is okay and disL is close and disR is close then vel is medium", engine));

    // Front distance medium
    mamdani->addRule(Rule::parse("if disF is medium and disL is okay and disR is okay then vel is medium", engine)); // slowL/R
    mamdani->addRule(Rule::parse("if disF is medium and disL is okay and disR is medium then vel is medium", engine));
    mamdani->addRule(Rule::parse("if disF is medium and disL is okay and disR is close then vel is medium", engine));
    mamdani->addRule(Rule::parse("if disF is medium and disL is medium and disR is okay then vel is medium", engine));
    mamdani->addRule(Rule::parse("if disF is medium and disL is medium and disR is medium then vel is medium", engine)); //slowL/R
    mamdani->addRule(Rule::parse("if disF is medium and disL is medium and disR is close then vel is medium", engine));
    mamdani->addRule(Rule::parse("if disF is medium and disL is close and disR is okay then vel is medium", engine));
    mamdani->addRule(Rule::parse("if disF is medium and disL is close and disR is medium then vel is medium", engine));
    mamdani->addRule(Rule::parse("if disF is medium and disL is close and disR is close then vel is slow", engine));

    // Front distance close
    mamdani->addRule(Rule::parse("if disF is close and disL is okay and disR is okay then vel is slow", engine)); // fastL/R
    mamdani->addRule(Rule::parse("if disF is close and disL is okay and disR is medium then vel is slow", engine));
    mamdani->addRule(Rule::parse("if disF is close and disL is okay and disR is close then vel is slow", engine));
    mamdani->addRule(Rule::parse("if disF is close and disL is medium and disR is okay then vel is slow", engine));
    mamdani->addRule(Rule::parse("if disF is close and disL is medium and disR is medium then vel is slow", engine)); // fastL/R
    mamdani->addRule(Rule::parse("if disF is close and disL is medium and disR is close then vel is slow", engine));
    mamdani->addRule(Rule::parse("if disF is close and disL is close and disR is okay then vel is slow", engine));
    mamdani->addRule(Rule::parse("if disF is close and disL is close and disR is medium then vel is slow", engine));
    mamdani->addRule(Rule::parse("if disF is close and disL is close and disR is close then vel is slow", engine)); // Maybe turn around - stuck situation

    engine->addRuleBlock(mamdani);

    // Check if engine is ready
    std::string status;
    if (not engine->isReady(&status))
        throw fl::Exception("[engine error] engine is not ready:n" + status, FL_AT);

}

void fuzzyController::runFuzzyController(float distFront, float distLeft, float distRight){

    //std::cout << distFront << std::endl;
    disF->setValue(distFront);
    disL->setValue(distLeft);
    disR->setValue(distRight);

    engine->process();

    outputVelocity = double(vel->getValue());
    outputDirection = double(dir->getValue());

    //FL_LOG("vel: " << Op::str(vel->getValue()) <<
         //      " => " << "dir: " << Op::str(dir->getValue()));
}


