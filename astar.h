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

    struct nodes {
      cv::Point pos;
      double g_cost;
      double h_cost;
      double f_cost;
      cv::Point cameFrom;
    };

    Astar(std::string ImageIn);
    void insertInOpen (int index, std::tuple<double, double, cv::Point>);
    void findPath(cv::Point start, cv::Point end);
    void addBrushfire(std::vector<std::vector<int>> values){brushfireValues = values;};
    bool notInClosed(cv::Point);
    bool notInOpen(cv::Point);
    bool notInFinal(cv::Point);
    void showOpenList();
    void showClosedList();
    bool notBoundary(cv::Point);
    void showImage(int);
    void finalPath();
    std::vector<nodes> cost1(nodes);
    void fcost(nodes);
    double getFinalDistance() {return finalDistance;}


private:
    cv::Mat imageOrg;
    std::vector<std::vector<int>> brushfireValues;
    cv::Point start, target;
    double finalDistance = 0;

    std::vector<nodes> openList, closedList;

    std::vector<std::tuple<double, double, cv::Point>> open, closed;
    std::vector<cv::Point> m_finalPath;

};

#endif // ASTAR_H
