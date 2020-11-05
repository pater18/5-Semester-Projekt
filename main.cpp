#include <iostream>
#include <vector>

#include <opencv2/opencv.hpp>

#include "particlefilter.h"

using namespace std;

int main()
{

    ParticleFilter pf(100);

    std::cout << pf.particleFilterInitialized() << std::endl;

    return 0;
}
