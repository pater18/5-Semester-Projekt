#include "astar.h"

#include <iostream>


using namespace std;

int main()
{
    Astar path("floor_plan.png");
    path.findPath(cv::Point(1,2), cv::Point(0,1));
//    path.cost(cv::Point(1,1), 's');
    path.cost(cv::Point(1,1), 't');
}
