#include "astar.h"
#include "pathplanning.h"

#include <iostream>


using namespace std;

int main()
{

    cv::Point startPoint = cv::Point(10,15);
    cv::Point endPoint = cv::Point(5,9);
    std::string image = "floor_plan.png";
    std::string image2 = "simple_plan.png";

    pathPlanning brush("simple_plan.png");
    brush.brushfire();

//    for(int i = 0; i < brush.getBrushfire().size(); i++){
//        for(int j = 0; j < brush.getBrushfire()[i].size(); j++){
//            std::cout << brush.getBrushfire()[i][j] << " ";
//        }
//        std::cout << std::endl;
//    }


    Astar path(image2);
    path.addBrushfire(brush.getBrushfire());
    path.findPath(startPoint, endPoint);
    path.showOpenList();
    path.showClosedList();
    path.finalPath();
    path.showImage();
//    path.showClosedList();



    return 0;

}
