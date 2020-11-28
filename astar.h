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
    void insertInOpen (int index, std::tuple<int, int, cv::Point>);
    void findPath(cv::Point start, cv::Point end);
    void f_cost(cv::Point currentPos);
    std::vector<std::pair<float, cv::Point>> cost(cv::Point currentPos, char );
    void addBrushfire(std::vector<std::vector<int>> values){brushfireValues = values;};
    bool notInClosed(cv::Point);
    bool notInOpen(cv::Point);
    bool notInFinal(cv::Point);
    void showOpenList();
    void showClosedList();
    bool notBoundary(cv::Point);
    void showImage();
    void finalPath();

private:
    cv::Mat imageOrg;
    std::vector<cv::Point> m_finalPath;
    std::vector<std::tuple<int, int, cv::Point>> open, closed;
    cv::Point start, target;
    std::vector<std::vector<int>> brushfireValues;
};

#endif // ASTAR_H
