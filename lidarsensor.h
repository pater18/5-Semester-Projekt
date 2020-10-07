#ifndef LIDARSENSOR_H
#define LIDARSENSOR_H
#include <gazebo/gazebo_client.hh>
#include <gazebo/msgs/msgs.hh>
#include <gazebo/transport/transport.hh>

#include <opencv2/opencv.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <bits/stdc++.h>

extern boost::mutex mutex;


class lidarSensor
{

public:
    lidarSensor();

    void runLidarSensor();

private:

    const int rightRange = 95, frontRange = 105, leftRange = 200;

    // Vectors with distances for each section
    float closestObjectLeft = 0;
    float closestObjectFront = 0;
    float closestObjectRight = 0;

    float obstacleCollisionThreshold = 1;

    void lidarCallback(ConstLaserScanStampedPtr &msg);

    gazebo::transport::Node nodeLidar;
    gazebo::transport::SubscriberPtr subLidar;



};

#endif // LIDARSENSOR_H
