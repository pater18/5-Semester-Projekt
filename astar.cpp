    #include "astar.h"


Astar::Astar(std::string ImageIn)
{
    // loading image
    imageOrg = cv::imread(ImageIn);
    if (imageOrg.empty()){
        std::cerr << "Could not load image" << std::endl;
    }
    openList.reserve(300);
}

void Astar::findPath(cv::Point startPoint, cv::Point end)
{
    start = startPoint;
    target = end;

    nodes node;
    node.pos = start;
    node.g_cost = 0;
    node.h_cost = sqrt(pow(target.x - startPoint.x, 2) + pow(target.y - (startPoint.y), 2));
    node.f_cost = node.g_cost + node.h_cost;
    node.cameFrom = start;

    openList.push_back(node);
    fcost(node);

    for (size_t i = 0; i < openList.size(); i++) {
        if (openList[i].pos == startPoint) {
            openList.erase(openList.begin() + i);
            break;
        }
    }

    closedList.push_back(node);

    int count = 0;
    while (node.pos != target) {
        node = openList[0];
        closedList.push_back(openList[0]);
        openList.erase(openList.begin());
        fcost(node);
//        showImage(1);

        count++;
    }
}

bool Astar::notInClosed(cv::Point position)
{

    for (size_t i = 0; i < closedList.size(); i++) {
        if (position == closedList[i].pos)
            return false;
    }
    return true;
}

bool Astar::notInOpen(cv::Point pointToCheck)
{
    signed int s = openList.size();

    if (s == (-1)) {
        return true;
    }

    for (size_t i = 0; i < openList.size(); i++) {
        if (pointToCheck == openList[i].pos)
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
    for (size_t i = 0; i < openList.size(); i++){
        std::cout << " f_cost: " << openList[i].f_cost  << ", h_cost: " << openList[i].h_cost << " Point: " << openList[i].pos <<
                     ", came from: " << openList[i].cameFrom << ", g_cost: " << openList[i].g_cost <<  std::endl;
    }
    std::cout << std::endl;
}

void Astar::showClosedList()
{
     std::cout << "Closed list: " << std::endl;
    for (size_t i = 0; i < closedList.size(); i++){
        std::cout << " f_cost: " << closedList[i].f_cost  << ", h_cost: " << closedList[i].h_cost << " Point: " << closedList[i].pos <<
                     ", came from: " << closedList[i].cameFrom << ", g_cost: " << closedList[i].g_cost <<  std::endl;
    }
    std::cout << std::endl;
}

bool Astar::notBoundary(cv::Point currentPoint)
{
    if ((unsigned long)currentPoint.x > brushfireValues[0].size() - 1)
        return false;
    else if ((unsigned long)currentPoint.y > brushfireValues.size() - 1)
        return false;
    else if (currentPoint.x < 1 || currentPoint.y < 1)
        return false;
    else {
        return true;
    }
}

void Astar::showImage(int sel)
{

     cv::Mat finalPath;
    finalPath = imageOrg.clone();


    if (sel == 1) {
        for (size_t x = 0; x < closedList.size(); x++) {
            finalPath.at<cv::Vec3b>(closedList[x].pos) = {0, 0, 255};
        }
        for (size_t x = 0; x < openList.size(); x++) {
            finalPath.at<cv::Vec3b>(openList[x].pos) = {0, 255, 0};
        }
        cv::namedWindow("Final *");
        cv::imwrite("astar.png", finalPath);
        cv::imshow("final *", finalPath);
        cv::waitKey(5);
    } else {
        for (size_t x = 0; x < closedList.size(); x++) {
            finalPath.at<cv::Vec3b>(closedList[x].pos) = {0, 0, 255};
        }
        for (size_t x = 0; x < openList.size(); x++) {
            finalPath.at<cv::Vec3b>(openList[x].pos) = {0, 255, 0};
        }
        for (size_t x = 0; x < m_finalPath.size(); x++) {
            finalPath.at<cv::Vec3b>(m_finalPath[x]) = {255, 0, 0};
        }
        cv::namedWindow("Final *");
        cv::imwrite("astar.png", finalPath);
        cv::imshow("final *", finalPath);
        cv::waitKey(0);
    }



}

void Astar::finalPath()
{
    std::vector<nodes> tempFinal = closedList;
    cv::Point cameFrom = tempFinal[tempFinal.size() - 1].cameFrom;
    m_finalPath.push_back(tempFinal[tempFinal.size() - 1].pos);
    for (size_t i = tempFinal.size() - 1; i > 0; i--) {
        if (cameFrom == tempFinal[i].pos) {
            m_finalPath.push_back(tempFinal[i].pos);
            cameFrom = tempFinal[i].cameFrom;
            finalDistance += tempFinal[i].g_cost;
        }
    }
}

std::vector<Astar::nodes> Astar::cost1( nodes currentNode)
{
    std::vector<nodes> costNodes;

    for(int x = -1; x <= 1; x++){
        for(int y = -1; y <= 1; y++){
            nodes node;
            if (y == 0 && x == 0)
                continue;
            if (notBoundary(cv::Point(currentNode.pos.x + x, currentNode.pos.y + y))) {
                if (brushfireValues[currentNode.pos.y + y][currentNode.pos.x + x] != 0 && notInClosed(cv::Point(currentNode.pos.x + x, currentNode.pos.y + y))) {
                    node.h_cost = sqrt(pow(target.x - (currentNode.pos.x + x), 2) + pow(target.y - (currentNode.pos.y + y), 2));
                    double g_costCurrentNodeToNext = sqrt(pow(currentNode.pos.x - currentNode.pos.x + x, 2) + pow(currentNode.pos.y - currentNode.pos.y + y, 2));
                    double totalGCost = currentNode.g_cost + g_costCurrentNodeToNext;
                    node.g_cost = totalGCost;
                    node.pos = cv::Point(currentNode.pos.x + x, currentNode.pos.y + y);
                    node.cameFrom = currentNode.pos;
                    node.f_cost = node.h_cost + node.g_cost;
                    costNodes.push_back(node);
                }
            }
        }
    }

    return costNodes;
}

void Astar::fcost(nodes currentNode)
{

    enum fcostCheck {
        greaterThan,
        equalTo,
        lessThan
    } fcostCheck;

    std::vector<nodes> currentPositionCost = cost1(currentNode);


    while (currentPositionCost.size()) {
        nodes nodeToCheck = currentPositionCost[0];

        if (notInOpen(nodeToCheck.pos)) {

            if (nodeToCheck.f_cost < openList[0].f_cost) {
                fcostCheck = lessThan;
            } else if (nodeToCheck.f_cost == openList[0].f_cost) {
                fcostCheck = equalTo;
            } else if (nodeToCheck.f_cost > openList[0].f_cost) {
                fcostCheck = greaterThan;
            }
            int index = 0;
            switch (fcostCheck) {
            case lessThan:
                openList.insert(openList.begin(), nodeToCheck);
                break;

            case equalTo:
                index = 0;
                while (nodeToCheck.f_cost == openList[index].f_cost && nodeToCheck.h_cost < openList[index].h_cost && index < openList.size()) { // Den må heller ikke gå ud over vektoren
                    index++;
                }
                openList.insert(openList.begin() + index - 1, nodeToCheck);
                break;

            case greaterThan:
                index = 0;
                while (nodeToCheck.f_cost > openList[index].f_cost && index < openList.size()) {
                    index++;
                }
                if (nodeToCheck.f_cost == openList[index].f_cost) {
                    while (nodeToCheck.f_cost == openList[index].f_cost && nodeToCheck.h_cost < openList[index].h_cost && index < openList.size() ) { // Den må heller ikke gå ud over vektoren
                        index++;
                    }
                }
                openList.insert(openList.begin() + index, nodeToCheck);

                break;

            }


        } else if (!notInOpen(nodeToCheck.pos)) {
            //Find index
            int index = 0;
            for (size_t i = 0; i < openList.size(); i++) {
                if (nodeToCheck.pos == openList[i].pos) {
                    index = i;
                    break;
                }
            }

            if (nodeToCheck.f_cost < openList[index].f_cost) {
                fcostCheck = lessThan;
            } else if (nodeToCheck.f_cost == openList[index].f_cost) {
                fcostCheck = equalTo;
            } else if (nodeToCheck.f_cost > openList[index].f_cost) {
                fcostCheck = greaterThan;
            }


            switch (fcostCheck) {
            case lessThan:
                openList[index] = nodeToCheck;
                break;

            case equalTo:
                if (nodeToCheck.h_cost <= openList[index].h_cost) {
                    openList[index] = nodeToCheck;
                }
                break;

            case greaterThan:
                // Wont change the node currently in the list to the new node if the f-cost to that point is worse
                break;

            }


        }





        currentPositionCost.erase(currentPositionCost.begin());

    }

}

