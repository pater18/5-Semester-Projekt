#include <iostream>
#include <opencv2/opencv.hpp>

#include "marbledetection.h"
#include "lidarsensor.h"
#include "callBackFunctions.h"
#include "fuzzycontroller.h"


const bool showLidar = false;
const bool showPose = false;
const bool showCamera = false;
const bool showStat = false;

boost::mutex mutex;



int main(int _argc, char **_argv) {



    // Load gazebo
    gazebo::client::setup(_argc, _argv);

    // Create our node for communication
    gazebo::transport::NodePtr node(new gazebo::transport::Node());
    node->Init();

    // Listen to Gazebo topics
    gazebo::transport::SubscriberPtr statSubscriber;
    gazebo::transport::SubscriberPtr poseSubscriber;
    gazebo::transport::SubscriberPtr cameraSubscriber;
    gazebo::transport::SubscriberPtr lidarSubscriber;

    //Display the different types of data/information/frames
    if(showStat){
     statSubscriber = node->Subscribe("~/world_stats", statCallback);
    }

    if(showCamera){
    cameraSubscriber = node->Subscribe("~/pioneer2dx/camera/link/camera/image", cameraCallback);
    }

    if(showPose){
    poseSubscriber = node->Subscribe("~/pose/info", poseCallback);
    }

    if(showLidar){
    lidarSubscriber = node->Subscribe("~/pioneer2dx/hokuyo/link/laser/scan", lidarCallback);
    }


    // Publish to the robot vel_cmd topic
    gazebo::transport::PublisherPtr movementPublisher = node->Advertise<gazebo::msgs::Pose>("~/pioneer2dx/vel_cmd");

    // Publish a reset of the world
    gazebo::transport::PublisherPtr worldPublisher = node->Advertise<gazebo::msgs::WorldControl>("~/world_control");


    gazebo::msgs::WorldControl controlMessage;
    controlMessage.mutable_reset()->set_all(true);
    worldPublisher->WaitForConnection();
    worldPublisher->Publish(controlMessage);



    // Create an object for marble detection
    MarbleDetection marble;
    marble.runMarbleDetection();

    // Create an object for lidar sensor
    lidarSensor lidar;
    lidar.runLidarSensor();


    std::tuple<float,float,float> ranges;
    float distRight = 0;
    float distFront = 0;
    float distLeft = 0;


    // Fuzzy Logic Controller
    Engine* engine = new Engine;
    engine->setName("ObstacleAvoidance");
    engine->setDescription("");

    // Inputs to the controller
    InputVariable* obstacle = new InputVariable;
    obstacle->setName("obstacle");
    obstacle->setDescription("");
    obstacle->setEnabled(true);
    obstacle->setRange(0.000, 1.000);
    obstacle->setLockValueInRange(false);
    obstacle->addTerm(new Ramp("left", 0.000, 1.000));
    obstacle->addTerm(new Ramp("right", 1.000, 0.000));
    engine->addInputVariable(obstacle);

    // Outputs from the controller
    OutputVariable* mSteer = new OutputVariable;
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
    RuleBlock* mamdani = new RuleBlock;
    mamdani->setName("mamdani");
    mamdani->setDescription("");
    mamdani->setEnabled(true);
    mamdani->setConjunction(fl::null);
    mamdani->setDisjunction(fl::null);
    mamdani->setImplication(new AlgebraicProduct);
    mamdani->setActivation(new General);
    mamdani->addRule(Rule::parse("if obstacle is left then mSteer is right", engine));
    mamdani->addRule(Rule::parse("if obstacle is right then mSteer is left", engine));
    engine->addRuleBlock(mamdani);


    std::string status;
    if (not engine->isReady(&status))
        throw fl::Exception("[engine error] engine is not ready:n" + status, FL_AT);

//    for (int i = 0; i <= 50; ++i){
//        scalar location = obstacle->getMinimum() + i * (obstacle->range() / 50);
//        obstacle->setValue(location);
//        engine->process();
//        FL_LOG("obstacle.input = " << Op::str(location) <<
//            " => " << "steer.output = " << Op::str(mSteer->getValue()));
//    }

    // keys to control robot
    const int key_left = 81;
    const int key_up = 82;
    const int key_down = 84;
    const int key_right = 83;
    const int key_esc = 27;

    // set speed and direction for the robot
    float speed = 0.0;
    float dir = 0.0;

    // Infinite Loop
    while (true) {


        ranges = lidar.getMinRanges();

        distRight = std::get<0>(ranges);
        distFront = std::get<1>(ranges);
        distLeft = std::get<2>(ranges);

        std::cout << "right: " << distRight << std::endl;
        std::cout << "front: " << distFront << std::endl;
        std::cout << "left: " << distLeft << std::endl;

        obstacle->setValue(distFront);
        engine->process();

        FL_LOG("obstacle.input = " << Op::str(obstacle->getValue()) <<
            " => " << "steer.output = " << Op::str(mSteer->getValue()));

        gazebo::common::Time::MSleep(10);

        int key = cv::waitKey(1);

        // if "q" pressed - terminate program
        if (key == key_esc){
          break;
        }

        if(key == 's'){
            speed = 0;
        }

        // robot controller with arrow keys
        if ((key == key_up) && (speed <= 1.2f))
          speed += 0.05;
        else if ((key == key_down) && (speed >= -1.2f))
          speed -= 0.05;
        else if ((key == key_right) && (dir <= 0.4f))
          dir += 0.05;
        else if ((key == key_left) && (dir >= -0.4f))
          dir -= 0.05;
        else {
          // slow down
          //      speed *= 0.1;
          //      dir *= 0.1;
        }


        // Generate a pose
        //ignition::math::Pose3d pose(double(speed), 0, 0, 0, 0, double(dir));

        ignition::math::Pose3d pose(double(speed), 0, 0, 0, 0, double(mSteer->getValue()));
        // Convert to a pose message
        gazebo::msgs::Pose msg;
        gazebo::msgs::Set(&msg, pose);
        movementPublisher->Publish(msg);
    }

    // Make sure to shut everything down.
    gazebo::client::shutdown();
}
