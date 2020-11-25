#include "astar.h"
#include "pathplanning.h"
#include "travel.h"

#include <limits>
#include <iostream>
#include <fstream>
#include <iomanip>


using namespace std;

long int factorial(int n) {

    if (n == 1) {
        return 1;
    }
    return n * factorial(n - 1);

}

std::vector<location> readLookUptable() {

    std::vector<location> lookuptable;
    std::ifstream read;
    read.open("lookuptable.txt");

    if (!read) {
        std::cout << "Unable to open file" << std::endl;
    }

    std::string line;
    while (std::getline(read, line)) {

        location newLocation;
        int index = 0;
        std::string str;

        std::pair<int, double> temp;

        while (index < line.length()) {
            while (line[index] != ' ' && line[index] != '[' && line[index] != ']') {
                str += line[index];
                index++;
            }

            if (index < 3 && str.length()) {
                newLocation.id = std::stoi(str);
                str.clear();
            } else if (index <= 12) {
                int x;
                int y;
                if (str.length()) {
                    x = std::stoi(str);
                    str.clear();
                    index ++;
                }
                while (line[index] != ' ' && line[index] != '[' && line[index] != ']') {
                    str += line[index];
                    index++;
                }
                if (str.length()) {
                    y = std::stod(str);
                    str.clear();
                    newLocation.point = cv::Point(x, y);
                }

            } else if (str.length() < 3 && str.length() > 0) {
                temp.first = std::stoi(str);
                str.clear();
            } else if (str.length() > 3) {
                temp.second = std::stod(str);
                newLocation.distance.push_back(temp);
                str.clear();
            }
            index++;
        }

        lookuptable.push_back(newLocation);

    }

    return lookuptable;

}

std::pair<double, std::vector<int>> readBest(std::string str){
    std::ifstream read;
    read.open(str);
//    read.open("lexiograf.txt");

    if (!read) {
        std::cout << "Unable to open file" << std::endl;
    }

    std::pair <double, std::vector<int>> result;
    std::string line;
    std::getline(read, line);
    double currentBestDistence = std::stod(line);
    result.first = currentBestDistence;


    std::vector<int> currentBestPath;
    std::string temp;
    std::getline(read, line);
    size_t index = 0;
    while (line.length() >= index) {

        if (line[index] != ' ' ) {
            temp += line[index];

        }
        else if (line[index] == ' ' && !temp.empty()) {
            currentBestPath.push_back(std::stoi(temp));
            temp.clear();
        }
        index++;
    }

    result.second = currentBestPath;

    return result;

}


void createLookUpTable () {
    //    //Alle punkter har fået et id nummer
    //    for (size_t i = 0; i < locations.size(); i++) {
    //        location newLocation;
    //        newLocation.id = i + 1;
    //        newLocation.point = locations[i];
    //        lookupTable.push_back((newLocation));
    //    }

    //    pathPlanning createBrushFire(image);
    //    createBrushFire.brushfire();

    //    // Finde the distance between all the points above
    //    for(size_t j = 0; j < lookupTable.size(); j++) {
    //        for(size_t k = j + 1; k < lookupTable.size(); k++) {

    //            // Start to find the distance between two points
    //            cv::Point currentPosition = locations[j];
    //            cv::Point targetPoint = locations[k];

    //            Astar path(image);
    //            path.addBrushfire(createBrushFire.getBrushfire());
    //            path.findPath(currentPosition, targetPoint);
    //            path.finalPath();

    //            //Add that distance to the lookup table
    //            lookupTable[j].distance.push_back(std::make_pair(k + 1, path.getFinalDistance()));
    //            std::cout << "j = " << j << " og k = " << k << std::endl;
    //        }
    //    }
    //    writeFile(lookupTable);
}

void writeFile (std::vector<location> lookupTable) {
//     Write out to a file all the distances
    ofstream outfile;
    outfile.open("lookuptable.txt");
    for (size_t i = 0; i < lookupTable.size(); i++) {
        outfile << lookupTable[i].id << " " << lookupTable[i].point << "   ";
        for (size_t j = 0; j < lookupTable[i].distance.size(); j++) {
            outfile << "[ " << lookupTable[i].distance[j].first << " " << lookupTable[i].distance[j].second << " ]" ;

        }
        outfile << std::endl;
    }
    outfile.close();
}

int main()
{
    std::string image = "floor_plan.png";



    std::vector <int> initialPath = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17};
    std::vector<cv::Point> locations = {cv::Point(9,8), cv::Point(25,16), cv::Point(10,25), cv::Point(42,15),       //0, 1, 2, 3
                                        cv::Point(68,9), cv::Point(68, 25), cv::Point(94,19), cv::Point(112,16),    //4, 5, 6, 7
                                        cv::Point(36,38), cv::Point(71,38), cv::Point(8,62), cv::Point(32,61),      //8, 9, 10, 11
                                        cv::Point(71,53), cv::Point(55,72), cv::Point(77,68), cv::Point(107,54),    //12, 13, 14, 15
                                        cv::Point(108,68)};                                                         //16

    std::vector<location> lookupTable;
    lookupTable = readLookUptable();

    std::pair<double, std::vector<int>> currentBest = readBest("lexiograf.txt");
    std::pair<double, std::vector<int>> toCheck = readBest("Current_Progress.txt");
    ofstream BestRoute;
    ofstream NextCheck;
    travel travel(toCheck.second);

    travel.getLookUptable(lookupTable);

    double currentBestDistance = currentBest.first;
    std::vector<int> currentBestPath = currentBest.second;
    std::vector<int> currentCombinationToCheck;


    long long int numberOfCombinationsTried = toCheck.first;
    while (1) {
        travel.lexi();
        if (currentBestDistance > travel.getBestDistance()) {
            currentBestDistance = travel.getBestDistance();
            currentBestPath = travel.getBestPath();

            BestRoute.open("Best_Route.txt");

            BestRoute << currentBestDistance << std::endl;
            std::cout << "The best distance: " << currentBestDistance << " with:" << std::endl;
            for (size_t i = 0; i < currentBestPath.size(); i++) {
                std::cout << currentBestPath[i] << " ";
                BestRoute << currentBestPath[i] << " ";
            }
            std::cout << std::endl;
            BestRoute << std::endl;
            BestRoute.close();

        }

        if (numberOfCombinationsTried % 1000000 == 0)
        {

            NextCheck.open("Current_Progress.txt");
            NextCheck << numberOfCombinationsTried << std::endl;

            std::cout << "Combinations tried: " << numberOfCombinationsTried << std::endl;
            std::cout << "Current combination being checked: " << std::endl;
            currentCombinationToCheck = travel.getPathToCheck();
            for (size_t i = 0; i < currentCombinationToCheck.size(); i++ ) {
                std::cout << currentCombinationToCheck[i] << " " ;
                NextCheck << currentCombinationToCheck[i] << " ";
            }

            std::cout << std::endl;


            long double top = (long double)numberOfCombinationsTried;
            long double buttom = 355687428100000;
            long double finished = (top/buttom) * 100;

            std::cout << "It is: " << std::fixed << std::setprecision(30) << finished << " % " << "finished" << std::endl;
            std::cout << std::endl;
            NextCheck.close();
        }

        numberOfCombinationsTried++;
    }
    BestRoute.close();



    return 0;

}
