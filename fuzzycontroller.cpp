#include "fuzzycontroller.h"

fuzzyController::fuzzyController()
{

}


void fuzzyController::setupFuzzyController(){

    // Fuzzy Logic Controller
    engine->setName("ObstacleAvoidance");
    engine->setDescription("");

    // Inputs to the controller
    obstacle->setName("obstacle");
    obstacle->setDescription("");
    obstacle->setEnabled(true);
    obstacle->setRange(0.000, 1.000);
    obstacle->setLockValueInRange(false);
    obstacle->addTerm(new Ramp("left", 0.000, 1.000));
    obstacle->addTerm(new Ramp("right", 1.000, 0.000));
    engine->addInputVariable(obstacle);

    // Outputs from the controller
    mSteer->setName("mSteer");
    mSteer->setDescription("");
    mSteer->setEnabled(true);
    mSteer->setRange(0.000, 1.000);
    mSteer->setLockValueInRange(false);
    mSteer->setAggregation(new Maximum);
    mSteer->setDefuzzifier(new Centroid(100));
    mSteer->setDefaultValue(fl::nan);
    mSteer->setLockPreviousValue(false);
    mSteer->addTerm(new Ramp("left", 1.000, 0.000));
    mSteer->addTerm(new Ramp("right", 0.000, 1.000));
    engine->addOutputVariable(mSteer);

    // Rule Block/Base for the controller
    mamdani->setName("mamdani");
    mamdani->setDescription("");
    mamdani->setEnabled(true);
    mamdani->setConjunction(fl::null);
    mamdani->setDisjunction(fl::null);
    mamdani->setImplication(new AlgebraicProduct);
    mamdani->setActivation(new General);
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

    engine->addRuleBlock(mamdani);

    // Check if engine is ready
    std::string status;
    if (not engine->isReady(&status))
        throw fl::Exception("[engine error] engine is not ready:n" + status, FL_AT);

}

void fuzzyController::runFuzzyController(){


}


