#include <iostream>
#include <vector>

#include <opencv2/opencv.hpp>

#include "particlefilter.h"

using namespace std;

int main()
{

    cv::Mat map;
    map = cv::imread("floor_plan.png");


    ParticleFilter pf(5);

    double std[] = {1,1,1};

    pf.initializeParticleFilter(0,0,0, std, map);

    pf.drawParticles(map);

    pf.lidarParticles(map);

    pf.drawLidarParticles(map);


    std::cout << pf.particleFilterInitialized() << std::endl;

    cv::resize(map, map, cv::Size(0,0), 5,5);

    cv::imshow("Map", map);

    char key;

    while(true){
        key = cv::waitKey(0);

        if(key == 'q'){
            break;
        }

    }

    return 0;
}
