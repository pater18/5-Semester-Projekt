#ifndef ASTAR_H
#define ASTAR_H


#include <gazebo/gazebo_client.hh>
#include <gazebo/msgs/msgs.hh>
#include <gazebo/transport/transport.hh>

#include <opencv2/opencv.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <vector>
#include <bits/stdc++.h>


class Astar
{
public:
    Astar(std::string ImageIn);
    void findPath(cv::Point start, cv::Point end);
    cv::Point f_cost(cv::Point currentPos);
    cv::Point cost(cv::Point currentPos, char );

private:
    cv::Mat imageOrg;
    std::vector<cv::Point> open, closed;
    cv::Point start, target;
};

#endif // ASTAR_H
