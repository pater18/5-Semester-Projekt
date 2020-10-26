#include "astar.h"

int getF_cost(std::tuple<int, int, cv::Point> vectorIndex){
    return std::get<0> (vectorIndex);
}

int getH_cost(std::tuple<int, int, cv::Point> vectorIndex){
    return std::get<1> (vectorIndex);
}

cv::Point getPoint(std::tuple<int, int, cv::Point> vectorIndex){
    return std::get<2> (vectorIndex);
}

void Astar::insertInOpen (int index, std::tuple<int, int, cv::Point> pointToInsert) {
    if (notInOpen(getPoint(pointToInsert)))
        open.insert(open.begin() + index, pointToInsert);
    else {
        for (size_t i = 0; i < open.size(); i++) {
            if (getF_cost(pointToInsert) < getF_cost(open[i]) && getPoint(pointToInsert) == getPoint(open[i])) {
                open[index] = pointToInsert;
                break;
            }
        }
    }
}

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


    open.push_back(std::make_tuple(10000 , 100000, startPoint));
    start = startPoint;
    target = end;
    f_cost(startPoint);
    cv::Point currentNode = start;
    closed.push_back(std::make_tuple(10000 , 100000, startPoint));


//    cv::Mat astar( 20, 20, CV_8UC3);
//    imageOrg.at<uchar>(currentNode) = 255;

    cv::namedWindow("astar");
    while (currentNode != end){
        currentNode = getPoint(open[0]);
        closed.push_back( open[0]);
        open.erase(open.begin());
        f_cost(currentNode);
    }
}

void Astar::f_cost(cv::Point currentPos)
{

    std::vector<std::tuple<int, int, cv::Point>> neigbourValues;
    std::vector<std::pair<float, cv::Point>> f_cost;
    std::vector<std::pair<float, cv::Point>> g_cost = cost(currentPos, 's');
    std::vector<std::pair<float, cv::Point>> h_cost = cost(currentPos, 't');
    f_cost.resize(g_cost.size());
    neigbourValues.resize(g_cost.size());

    for (size_t i = 0; i < cost(currentPos, 't').size(); i++) {
        neigbourValues[i] = std::make_tuple(g_cost[i].first + h_cost[i].first ,h_cost[i].first, h_cost[i].second);

    }


    int iterator = 0;
    while (neigbourValues.size()) {

        if(getF_cost(neigbourValues[0]) < getF_cost(open[0])) {
            insertInOpen(0, neigbourValues[0]);

        } else if (getF_cost(neigbourValues[0]) == getF_cost(open[0])) {

            if(getH_cost(neigbourValues[0]) < getH_cost(open[0])) {
                insertInOpen(0, neigbourValues[0]);
            } else if (getH_cost(neigbourValues[0]) == getH_cost(open[0])) {
                insertInOpen(0, neigbourValues[0]);
            } else if (getH_cost(neigbourValues[0]) > getH_cost(open[0])) {
                while (getH_cost(neigbourValues[0]) > getH_cost(open[iterator++]));
                insertInOpen(iterator - 1, neigbourValues[0]);
            }


        } else if (getF_cost(neigbourValues[0]) > getF_cost(open[0])) {
            iterator = 0;
            while (getF_cost(neigbourValues[0]) > getF_cost(open[iterator++]));
            int iteratorH_cost = iterator - 1 ;

            if (getF_cost(neigbourValues[0]) == getF_cost(open[iteratorH_cost])) {

                if (getH_cost(neigbourValues[0]) < getH_cost(open[iteratorH_cost])) {
                    insertInOpen(iterator, neigbourValues[0]);
                } else if (getH_cost(neigbourValues[0]) == getH_cost(open[iteratorH_cost])) {
                    insertInOpen(iterator, neigbourValues[0]);
                } else if (getH_cost(neigbourValues[0]) > getH_cost(open[iteratorH_cost])) {
                    while (getH_cost(neigbourValues[0]) > getH_cost(open[iteratorH_cost++]));
                    insertInOpen(iteratorH_cost - 1, neigbourValues[0]);
                }

            } else {
                insertInOpen(iterator - 1, neigbourValues[0]);
            }
        }

        iterator = 0;
        neigbourValues.erase(neigbourValues.begin());
    }
}




std::vector<std::pair<float, cv::Point>> Astar::cost(cv::Point currentPos, char selPoint )
{
    std::vector<std::pair<float, cv::Point>> costCheck;

    for(int x = -1; x <= 1; x++){
        for(int y = -1; y <= 1; y++){

            if (y == 0 && x == 0)
                continue;

            if (selPoint == 't'){
                if (brushfireValues[currentPos.y + y][currentPos.x + x] != 0 && notInClosed(cv::Point(currentPos.x + x, currentPos.y + y)) && notBoundary(cv::Point(currentPos.x + x, currentPos.y + y))){
                    std::pair temp1 = std::make_pair( abs(target.x - (currentPos.x + x)) + abs(target.y - (currentPos.y + y)), cv::Point(currentPos.x + x, currentPos.y + y));
                    costCheck.push_back(temp1);
                }
            }
            else if (selPoint == 's'){
                if (brushfireValues[currentPos.y + y][currentPos.x + x] != 0 && notInClosed(cv::Point(currentPos.x + x, currentPos.y + y)) && notBoundary(cv::Point(currentPos.x + x, currentPos.y + y))){
                    std::pair temp = std::make_pair( abs(start.x - (currentPos.x + x)) + abs(start.y - (currentPos.y + y)), cv::Point(currentPos.x + x, currentPos.y + y));
                    costCheck.push_back(temp);
                }
            }
        }
    }
    return costCheck;
}

bool Astar::notInClosed(cv::Point position)
{
    for (size_t i = 0; i < closed.size(); i++) {
        if (position == std::get<2>(closed[i]))
            return false;
    }
    return true;
}

bool Astar::notInOpen(cv::Point pointToCheck)
{
    for (size_t i = 0; i < open.size(); i++) {
        if (pointToCheck == getPoint(open[i]))
            return false;
    }
    return true;
}

bool Astar::notInFinal(cv::Point pointToCheck)
{
    for (size_t i = 0; i < m_finalPath.size(); i++) {
        if (pointToCheck == m_finalPath[i])
            return false;
    }
    return true;
}

void Astar::showOpenList()
{
    std::cout << "Open list: " << std::endl;
    for (size_t i = 0; i < open.size(); i++){
        std::cout << " f_cost: " << std::get<0>(open[i])  << ", h_cost: " << std::get<1>(open[i]) << " Point: " << std::get<2>(open[i]) << std::endl;
    }
    std::cout << std::endl;
}

void Astar::showClosedList()
{
     std::cout << "Closed list: " << std::endl;
    for (size_t i = 0; i < closed.size(); i++){
        std::cout << " f_cost: " << std::get<0>(closed[i])  << ", h_cost: " << std::get<1>(closed[i]) << " Point: " << std::get<2>(closed[i]) << std::endl;
    }
    std::cout << std::endl;
}

bool Astar::notBoundary(cv::Point currentPoint)
{
    if ((unsigned long)currentPoint.x >= brushfireValues.size() - 1) {
        return false;
    } else if ((unsigned long)currentPoint.y >=brushfireValues[0].size() - 1)
        return false;
    else {
        return true;
    }


}

void Astar::showImage()
{
    cv::Mat astar;
    astar = imageOrg.clone();

    for (size_t x = 0; x < closed.size(); x++) {
        astar.at<cv::Vec3b>(getPoint(closed[x])) = {0,0,255};
    }
    for (size_t x = 0; x < open.size(); x++) {
        astar.at<cv::Vec3b>(getPoint(open[x])) = {0,255,0};
    }


    cv::namedWindow("astar");
    cv::imshow("astar", astar);
    cv::imwrite("astar.png", astar);
    cv::waitKey(0);
}

void Astar::finalPath()
{
    std::vector<std::tuple<int, int, cv::Point>> closedPath = closed;
    cv::Point position = target;
    m_finalPath.push_back(position);
    for (size_t i = 0; i < closedPath.size(); i++) {
        if (position == getPoint(closedPath[i]))
            closedPath.erase(closedPath.begin() + i);

    }

    std::cout << "closedpath is equal to: " << std::endl;
    for (size_t i = 0; i < closedPath.size(); i++) {
        std::cout << getPoint(closedPath[i]) << std::endl;
    }
    std::cout << std::endl << std::endl;

    int count = 0;
    while (count < 30/*position != start*/) {
        std::cout << "Position to check neigbour: " << position << std::endl;
        std::vector<std::tuple<int, int, cv::Point>> temp;

        cv::Point positionUp = cv::Point(position.x, position.y - 1);
        cv::Point positionUpRight = cv::Point(position.x + 1, position.y - 1);
        cv::Point positionUpLeft = cv::Point(position.x - 1, position.y - 1);
        cv::Point positionDown = cv::Point(position.x, position.y + 1);
        cv::Point positionDownRight = cv::Point(position.x + 1, position.y + 1);
        cv::Point positionDownLeft = cv::Point(position.x - 1, position.y + 1);
        cv::Point positionRight = cv::Point(position.x + 1, position.y);
        cv::Point positionLeft = cv::Point(position.x - 1, position.y);

        for (size_t i = 0; i < closedPath.size(); i++) {
            if (positionUp == getPoint(closedPath[i]) ) {
                temp.push_back(closedPath[i]);
            }
            if (positionUpLeft == getPoint(closedPath[i])) {
                temp.push_back(closedPath[i]);
            }
            if (positionUpRight == getPoint(closedPath[i])) {
                temp.push_back(closedPath[i]);
            }
            if (positionDown == getPoint(closedPath[i])) {
                temp.push_back(closedPath[i]);
            }
            if (positionDownLeft == getPoint(closedPath[i])) {
                temp.push_back(closedPath[i]);
            }
            if (positionDownRight == getPoint(closedPath[i])) {
                temp.push_back(closedPath[i]);
            }
            if (positionLeft == getPoint(closedPath[i])) {
                temp.push_back(closedPath[i]);
            }
            if (positionRight == getPoint(closedPath[i])) {
                temp.push_back(closedPath[i]);
            }
        }


        if (temp.size() == 1 /*&& notInFinal(getPoint(temp[0]))*/) {
            position = getPoint(temp[0]);
        } else {
            for (size_t i = 0; i < temp.size() - 1; i++) {
                if (getF_cost(temp[i]) < getF_cost(temp[i + 1])) {
                    position = getPoint(temp[i]);
                } else if (getF_cost(temp[i]) == getF_cost(temp[i + 1])) {
                    if (getH_cost(temp[i]) > getH_cost(temp[i + 1])) {
                        position = getPoint(temp[i]);
                    } else {
                        position = getPoint((temp[i + 1]));
                    }
                }
            }

        }

        std::cout << "Temp is equal to: " << std::endl;
        for (size_t i = 0; i < temp.size(); i++) {
            std::cout << "Fcost: " << getF_cost((temp[i])) << ", point: " <<getPoint(temp[i]) << ", " << ", Hcost: " << getH_cost(temp[i]) << std::endl;
        }
        std::cout << std::endl << std::endl;

        std::cout << "Position after check: " << position << std::endl << std::endl;

        for (size_t i = 0; i < closedPath.size(); i++) {
            if (position == getPoint(closedPath[i]))
                closedPath.erase(closedPath.begin() + i);
        }

        std::cout << "closedpath after erase is equal to: " << std::endl;
        for (size_t i = 0; i < closedPath.size(); i++) {
            std::cout << getPoint(closedPath[i]) << std::endl;
        }
        std::cout << std::endl << std::endl;

        m_finalPath.push_back(position);

//        std::cout << "Final path is equal to: " << std::endl;
//        for (size_t i = 0; i < m_finalPath.size(); i++) {
//            std::cout << m_finalPath[i] << " " ;
//        }
//        std::cout << std::endl << std::endl;

        count ++;
    }

    std::cout << "Final path is equal to: " << std::endl;
    for (size_t i = 0; i < m_finalPath.size(); i++) {
        std::cout << m_finalPath[i] << " " ;
    }
    std::cout << std::endl << std::endl;
}
