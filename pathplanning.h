#ifndef PATHPLANNING_H
#define PATHPLANNING_H

#include <gazebo/gazebo_client.hh>
#include <gazebo/msgs/msgs.hh>
#include <gazebo/transport/transport.hh>

#include <opencv2/opencv.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <vector>
#include <bits/stdc++.h>

class pathPlanning
{
public:
    pathPlanning();

private:
    int black = 0;
    int white = 255;
    std::vector<std::vector<int>> brushValues;

};

#endif // PATHPLANNING_H
