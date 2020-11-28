#include <iostream>
#include <opencv2/opencv.hpp>
#include <typeinfo>
#include <cmath>

#include "marbledetection.h"
#include "lidarsensor.h"
#include "callBackFunctions.h"
#include "fuzzycontroller.h"
#include "pose.h"


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
    //MarbleDetection marble;
    //marble.runMarbleDetection();

    // Create an object for lidar sensor
    lidarSensor lidar;
    lidar.runLidarSensor();

    std::tuple<float,float,float> ranges;
    float distRight = 10.0;
    float distFront = 10.0;
    float distLeft = 10.0;
    float distMinCon = 1.5;

    // Create an object and set up both fuzzy controllers
    fuzzyController controller;
    controller.setupFuzzyController();
    fuzzyController controllerGoal;
    controllerGoal.setupFuzzyControllerGoal();

    // Create an object to get poseCallback info for robot
    pose Pose;
    Pose.runPose();


    // keys to control robot
    const int key_left = 81;
    const int key_up = 82;
    const int key_down = 84;
    const int key_right = 83;
    const int key_esc = 27;

    // initialize robot states
    bool emergencyStop = false;
    bool goalReached = false;
    //    bool distToFar = false;

    // set speed and direction for the robot
    float speed = 0.0;
    double dir = 0.0;

    // set goal position and get the angle from rob to point
    double goalPosX = 38.0;
    double goalPosY = 22.0;
    double angleRobToPoint = 0.0;
    double angleYaw = 0.0;
    double inputAngle = 0.0;
//    double distToPointOld = std::sqrt(std::pow(goalPosY - Pose.getPosY(),2) + std::pow(goalPosX - Pose.getPosX(),2));
//    double distToPointNew = 0.0;

    std::vector<cv::Point2d> robToGoalPath;
    int counter = 0;

    // Infinite Loop
    while (!goalReached) {

        // get lidar minimum ranges from the three different directions
        ranges = lidar.getMinRanges();
        distRight = std::get<0>(ranges);
        distFront = std::get<1>(ranges);
        distLeft = std::get<2>(ranges);


        // Distance to marble
        //marble.distanceToMarbleOld(&lidar);
        //marble.distanceToMarble(distFront);


        // Fuzzy controller angle calculation for go to goal point
        angleRobToPoint = std::atan2(goalPosY - Pose.getPosY(), goalPosX - Pose.getPosX());
        angleYaw = Pose.getYaw();

        if ( (angleRobToPoint > 0 && angleYaw < -1.7) || (angleRobToPoint < 0 && angleYaw > 1.7) ) {
            inputAngle = angleYaw - angleRobToPoint;
        } else if (angleRobToPoint > 0) {
            inputAngle = angleRobToPoint - std::abs(angleYaw);
        } else if (angleRobToPoint < 0) {
            inputAngle = angleRobToPoint - angleYaw;
        }


        std::cout << "posX: " << Pose.getPosX() << " posY: " << Pose.getPosY() << " yaw: " << angleYaw << std::endl;
        std::cout << "angle: " << angleRobToPoint << std::endl;
        std::cout << "angle-yaw: " << inputAngle << std::endl;



        gazebo::common::Time::MSleep(10);

        int key = cv::waitKey(1);

        // if "q" pressed - terminate program
        if (key == key_esc){
          break;
        }



        // robot controller with arrow keys
//        if ((key == key_up))
//          speed += 0.05;
//        else if ((key == key_down))
//          speed -= 0.05;
//        else if ((key == key_right) && (dir <= 0.4f))
//          dir += 0.05;
//        else if ((key == key_left) && (dir >= -0.4f))
//          dir -= 0.05;
//        else {
//          // slow down
//          //      speed *= 0.1;
//               //dir *= 0.1;
//        }


        if (goalPosX == int(Pose.getPosX()) && goalPosY == int(Pose.getPosY())) {
            goalReached = true;
        }

        if(key == 's'){
            emergencyStop = true;
        }

        if(key == 'r'){
            emergencyStop = false;
        }


//        distToPointNew = std::sqrt(std::pow(goalPosY - Pose.getPosY(),2) + std::pow(goalPosX - Pose.getPosX(),2));

//        if ( distToPointNew > distToPointOld + 2 ) {
//            distToFar = true;
//        }

//        if ( distToPointNew < distToPointOld - 5) {
//            distToPointOld = distToPointNew;
//        }

//        if ( distToPointNew > distToPointOld + 2 ) {
//            speed = 0.0;
//            dir = controllerGoal.getOutputDirectionGoal();
//            distToPointOld = distToPointNew;
//            std::cout << "stop robot and rotate" << std::endl;
//        }


        // Choose which fuzzy controller to run
        if(distRight < distMinCon || distFront < distMinCon || distLeft < distMinCon){
            controller.runFuzzyController(distFront, distLeft, distRight);
        } else {
            controllerGoal.runFuzzyControllerGoal(inputAngle);
        }


        if(emergencyStop){
            speed = 0.0;
            dir = 0.0;
            std::cout << "emergency stop" << std::endl;
        } else if (goalReached) {
            dir = 0.0;
            speed = 0.0;
            std::cout << "goal reached" << std::endl;
        }
//        else if (distToFar) {
//            dir = controllerGoal.getOutputDirectionGoal();
//            speed = 0.0;
//            std::cout << "stop robot and rotate" << std::endl;
//            if (key == 'r') {
//                distToPointOld = distToPointNew;
//                distToFar = false;
//                std::cout << "distToFar: " << distToFar << std::endl;
//            }
//            if (inputAngle < 0.005 && inputAngle > -0.005) {
//                distToPointOld = distToPointNew;
//                distToFar = false;
//                std::cout << "distToFar: " << distToFar << std::endl;
//            }
//        }
        else if((distRight < distMinCon && distRight != 0) || (distFront <= distMinCon && distFront != 0) || (distLeft < distMinCon && distLeft != 0)){
            dir = controller.getOutputDirection();
            speed = controller.getOutputVelocity();
            std::cout << "controller obstacle avoidance" << std::endl;
        } else if((distRight > distMinCon && distRight != 0) || (distFront > distMinCon && distFront != 0) || (distLeft > distMinCon && distLeft != 0)){
            dir = controllerGoal.getOutputDirectionGoal();
            speed = controllerGoal.getOutputVelocityGoal();
            std::cout << "controller goal" << std::endl;
        }


        std::cout << "inputAngle: " << inputAngle << std::endl;
        std::cout << "speed: " << speed << std::endl;
        std::cout << "dir: " << dir << std::endl;


        // Generate a pose
        ignition::math::Pose3d pose(double(speed), 0, 0, 0, 0, double(dir));

        counter++;

        if (counter == 20) {
            robToGoalPath.push_back(cv::Point2d(Pose.getPosX(), Pose.getPosY()));
            counter = 0;
        }

        // Convert to a pose message
        gazebo::msgs::Pose msg;
        gazebo::msgs::Set(&msg, pose);
        movementPublisher->Publish(msg);

    }


    cv::Mat finalPath = cv::imread("floor_plan.png");

    std::cout << "robToGoalPath: ";
    for(size_t i = 0; i < robToGoalPath.size(); i++) {
        robToGoalPath[i].x = (robToGoalPath[i].x*1.5 + finalPath.cols/2);
        robToGoalPath[i].y *= -1;
        robToGoalPath[i].y = (robToGoalPath[i].y*1.5 + finalPath.rows/2);
    }
    std::cout << std::endl;

    std::cout << "row: " << finalPath.rows << " col: " << finalPath.cols << std::endl;

    for(size_t i = 0; i < robToGoalPath.size(); ++i) {
        cv::circle(finalPath, robToGoalPath[i], 0, cv::Scalar(0,0,255), 1);
        //cv::line(finalPath, robToGoalPath[i], robToGoalPath[i+1], cv::Scalar(255,0,0), 1);
    }

    cv::namedWindow("finalPath");
    cv::resize(finalPath, finalPath, cv::Size(finalPath.cols*5, finalPath.rows*5), 0, 0, cv::INTER_NEAREST);
    cv::imshow("finalPath", finalPath);
    cv::imwrite("finalPath.png", finalPath);
    cv::waitKey(0);

    // Make sure to shut everything down.
    gazebo::client::shutdown();
}
