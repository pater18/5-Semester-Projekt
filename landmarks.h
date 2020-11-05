#ifndef LANDMARKS_H
#define LANDMARKS_H

#endif // LANDMARKS_H

#include <iostream>
#include <vector>



// LandmarkObservation and single landmark could be combined into 1 class.

struct LandmarkObservation {

    int id;
    double x;
    double y;

};


class Map {
public:

    struct single_landmark_s{

        int id ; // Landmark ID
        double x; // Landmark x-position in the map (global coordinates)
        double y; // Landmark y-position in the map (global coordinates)
    };

    std::vector<single_landmark_s> landmark_list ; // List of landmarks in the map

};
