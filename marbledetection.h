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


extern boost::mutex mutex;

class MarbleDetection
{
public:
    MarbleDetection();
    double getMarbleAngle(){return marbleAngle;}
    double getMarbleDistance(){return marbleDistance;}
    std::pair<int, int> getMarbleXY(){return marbleXY;}
    bool isMarbleDetected(){return !circles.empty();}

    void detectMarbles();
    void cameraCallback(ConstImageStampedPtr &msg);

private:
    std::pair<int, int> marbleXY;
    double marbleAngle = 1;
    double marbleDistance = 1;
    bool marbleDetected = false;
    std::vector<cv::Vec3f> circles;

    gazebo::transport::Node node;
    gazebo::transport::PublisherPtr pub;
    gazebo::transport::SubscriberPtr sub;

    void houghDetection(const cv::Mat &gray, const cv::Mat &display, int cannyValue, int accumulatorValue);
};

#endif // MARBLEDETECTION_H
