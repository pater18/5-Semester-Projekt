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

    ParticleFilter pf(1000);

    double std[] = {2,2,2};

    Particle robot = {0, map.rows/2, map.cols/2, 180, 1.0};

    for(auto & range : robot.lidar){
        range = 10.0;
    }

    cv::imshow("Map", map);

    // keys to control robot
    const int key_left = 81;
    const int key_up = 82;
    const int key_down = 84;
    const int key_right = 83;
    const int key_esc = 27;

    // set speed and direction for the robot
    float speed = 0.0;
    double dir = 0.0;
    double oldDir = 0.0;
    double orientation_rate = 0.0;

    bool run = false;

    int numberOfResample = 300;
    double stdPos = 10.0;

    while(true){


        if(!pf.particleFilterInitialized()){
            pf.initializeParticleFilter(0,0,0, std, map);
        }


        int key = cv::waitKey(1);


        // robot controller with arrow keys
        if ((key == key_up))
          speed += 0.1;
        else if ((key == key_down) && (speed >= -1.2f))
          speed -= 0.1;
        else if ((key == key_right) && (dir <= 360.0))
          dir += 10.0;
        else if ((key == key_left) && (dir >= 0.0))
          dir -= 10.0;
        else {
          // slow down
                speed *= 0.99;
               //dir *= 0.1;
        }


//        if(dir != oldDir){
//            dir -= oldDir;
//        }

        //std::cout << "oldDir: " << oldDir << std::endl;
        //std::cout << "Dir: " << dir << std::endl;


        if(key == 'r'){
            run = true;
        }

        if(run == true){

            cv::Mat map;
            map = cv::imread("floor_plan.png");

           // cv::resize(map, map, cv::Size(0,0), 5,5 );


            orientation_rate = dir - oldDir;

//            std::cout << "speed: " << speed << std::endl;
//            std::cout << "Dir: " << dir << std::endl;
//            std::cout << "Orien: " << orientation_rate << std::endl;

            pf.prediction(1, 0.5, speed, orientation_rate, robot);

            pf.moveRobot(1, 0.5, speed, orientation_rate, robot);

            oldDir = dir;

            pf.lidarParticles(map);

            pf.robotLidar(robot, map);

            // Draw
            pf.drawParticles(map);

            //pf.drawLidarParticles(map);

            pf.drawRobotParticle(robot, map);

            // Find the good particles when compared to the robots view
            pf.associateParticlesWithRobot(robot);

//            while(true){
//                key = cv::waitKey(0);

//                if(key == 'q'){
//                    break;
//                }
//            }

            if(key == 's'){
                pf.resampleParticles(numberOfResample, stdPos);
                if(numberOfResample > 50){
                    stdPos -= 1.0;
                    numberOfResample -= 30;
                } else {
                    numberOfResample = 50;
                    stdPos = 2.0;
                }

            }


            //stdPos -= 0.01;
            //pf.lidarParticles(map);

            //pf.drawParticles(map);

            pf.drawLidarParticles(map);

            //cv::resize(resample, resample, cv::Size(0,0), 5,5);
            cv::imshow("Map", map);

//            while(true){
//                key = cv::waitKey(0);

//                if(key == 'q'){
//                    break;
//                }
//            }
        }
    }

    return 0;
}
