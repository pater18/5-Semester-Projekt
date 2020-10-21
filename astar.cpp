#include "astar.h"

Astar::Astar(std::string ImageIn)
{
    // loading image
    imageOrg = cv::imread(ImageIn);
    if (imageOrg.empty()){
        std::cerr << "Could not load image" << std::endl;
    }
}




void Astar::findPath(cv::Point startPoint, cv::Point end)
{
    open.push_back(startPoint);
    start = startPoint;
    target = end;

}

cv::Point Astar::f_cost(cv::Point currentPos)
{



}


cv::Point Astar::cost(cv::Point currentPos, char selPoint )
{
    std::vector<std::pair<float, cv::Point>> costCheck;
    cv::Point lowestCost;

    for (size_t i = 0; i < costCheck.size(); i++) {
        std::cout << "Before " << costCheck[i].first << " " << costCheck[i].second << std::endl;
    }
    std::cout << std::endl;

    std::vector<std::vector<int>> brushValues {{0,2,3},{4,5,6 },{7,8,9}};

    for(int x = -1; x <= 1; x++){
        for(int y = -1; y <= 1; y++){
            if (y == 0 && x == 0)
                continue;

            if (selPoint == 't'){
                std::pair temp1 = std::make_pair( abs(target.x - (currentPos.x + x)) + abs(target.y - (currentPos.y + y)), cv::Point(currentPos.x + x, currentPos.y + y));
                if (brushValues[currentPos.y + y][currentPos.x + x] != 0){
                    costCheck.push_back(temp1);
                }
            }
            else if (selPoint == 's'){
                std::pair temp = std::make_pair( abs(start.x - (currentPos.x + x)) + abs(start.y - (currentPos.y + y)), cv::Point(currentPos.x + x, currentPos.y + y));
                if (brushValues[currentPos.y + y][currentPos.x + x] != 0){
                    costCheck.push_back(temp);
                }
            }
        }
    }

    for (size_t i = 0; i < costCheck.size(); i++) {
        std::cout << costCheck[i].first << " " << costCheck[i].second << std::endl;
    }
    std::cout << std::endl;

    bool doneSort = false;
    while(!doneSort){
        doneSort = true;
        for (size_t i = 0; i < costCheck.size() - 1; i++) {

            if (costCheck[i].first > costCheck[i + 1].first){
                std::swap(costCheck[i], costCheck[i+1]);
                doneSort = false;
            }
        }
    }

    for (size_t i = 0; i < costCheck.size(); i++) {
        std::cout << costCheck[i].first << " " << costCheck[i].second << std::endl;
    }
    std::cout << std::endl;

    lowestCost = costCheck[0].second;
    return lowestCost;
}
