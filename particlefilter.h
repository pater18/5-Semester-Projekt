#ifndef PARTICLEFILTER_H
#define PARTICLEFILTER_H

#include "landmarks.h"

#include <iostream>
#include <vector>

struct Particle {

    int id;
    double x;
    double y;
    double orientation;
    double weight;
    std::vector<double> associations;
    std::vector<double> sense_x;
    std::vector<double> sense_y;

};


class ParticleFilter
{
public:
    ParticleFilter();
    ParticleFilter(int numberOfParticles)
        :numberOfParticles(numberOfParticles) {}


    std::vector<Particle> particles;


    bool particleFilterInitialized(){ return particleFilterInit; }
    std::vector<double> getParticleWeights(){ return weights; }

    // Function to run the particle filter on map. Implement as the last thing if its possible
    void runParticleFilter(int map);

    // Function for particle filter
    void initializeParticleFilter(double x, double y, double orientation, double std[]);

    void prediction(double delta_timestep, double stdPos, double velocity, double orientation_rate);

    void associateData(const std::vector<LandmarkObservation> &predicted, std::vector<LandmarkObservation> &observations);

    void updateWeights(double lidar_range, double stdLandmark[], const std::vector<LandmarkObservation> &observations, const Map &map_landmarks);

    void printParticles(Particle particles);

    void resampleParticles();

    void setAssociations(Particle& particle, std::vector<double> new_associations, std::vector<double> new_sense_x, std::vector<double> new_sense_y);

    std::vector<double> getAssociations(Particle best) { return best.associations; }
    std::vector<double> getSenseX(Particle best) { return best.sense_x; }
    std::vector<double> getSenseY(Particle best) {return best.sense_y; }


private:

    int numberOfParticles = 0;
    bool particleFilterInit = false;

    std::vector<double> weights;

};

#endif // PARTICLEFILTER_H
