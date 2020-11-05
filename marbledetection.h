#ifndef MARBLEDETECTION_H
#define MARBLEDETECTION_H
#include <gazebo/gazebo_client.hh>
#include <gazebo/msgs/msgs.hh>
#include <gazebo/transport/transport.hh>

#include <opencv2/opencv.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <bits/stdc++.h>

#include "lidarsensor.h"


extern boost::mutex mutex;

class MarbleDetection
{
public:
    MarbleDetection();

    void runMarbleDetection();

    std::pair<int, int> getMarbleXY(){return marbleXY;}
    double getMarbleAngle(){return marbleAngleHorizontal;}
    double getMarbleDistance(){return marbleDistance;}
    bool isMarbleDetected(){return !circles.empty();}

    // Get distance form lidar object
    void distanceToMarbleOld(lidarSensor *lidar);
    // Get disstance in main
    void distanceToMarble(float distFront);

private:

    std::pair<int, int> marbleXY;

    double marbleAngleHorizontal = 0;
    double marbleDistance = 0;
    double marbleDistanceCamera = 0;

    bool marbleDetected = false;
    std::vector<cv::Vec3f> circles = {};

    int oldRadius = 0;


    gazebo::transport::Node nodeCamera;
    gazebo::transport::SubscriberPtr subCamera;


    void detectMarbles();
    void cameraCallback(ConstImageStampedPtr &msg);
    void houghDetection(const cv::Mat &gray, const cv::Mat &imgOutput, int cannyValue, int accumulatorValue);
    void calculateAngleToMarble(std::vector<cv::Vec3f> circles, int closestCircleDetected);
    void calculateDistanceToMarbleFromCamera(int radiusClosest);

};

#endif // MARBLEDETECTION_H
