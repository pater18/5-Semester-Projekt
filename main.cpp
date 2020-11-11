#include <iostream>
#include <vector>

#include <opencv2/opencv.hpp>

#include "particlefilter.h"

using namespace std;

int main()
{


    cv::Mat map;
    map = cv::imread("floor_plan.png");

    char key;

    ParticleFilter pf(50);

    double std[] = {3,3,3};

    Particle robot = {0, map.cols/4, map.rows/2, 1, 1};

    for(auto & range : robot.lidar){
        range = 10.0;
    }

    // Init
    pf.initializeParticleFilter(0,0,0, std, map);

    pf.prediction(1, 5, 0.2, 0.1);

    pf.lidarParticles(map);

    pf.robotLidar(robot, map);

    // Draw
    pf.drawParticles(map);

    pf.drawLidarParticles(map);

    pf.drawRobotParticle(robot, map);


    pf.associateParticlesWithRobot(robot);

    cv::resize(map, map, cv::Size(0,0), 5,5);
    cv::imshow("Map", map);

    while(true){
        key = cv::waitKey(0);

        if(key == 'q'){
            break;
        }

    }

    cv::Mat resample;
    resample = cv::imread("floor_plan.png");

    pf.resampleParticles();

    pf.lidarParticles(resample);

    pf.drawParticles(resample);

    pf.drawLidarParticles(resample);

    cv::resize(resample, resample, cv::Size(0,0), 5,5);
    cv::imshow("resample", resample);

    while(true){
        key = cv::waitKey(0);

        if(key == 'q'){
            break;
        }

    }

    return 0;
}
