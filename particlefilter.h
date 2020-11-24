#ifndef PARTICLEFILTER_H
#define PARTICLEFILTER_H

#include "landmarks.h"

#include <opencv2/opencv.hpp>

#include <iostream>
#include <vector>
#include <random>

struct Particle {

    int id;
    double x;
    double y;
    double orientation;
    double weight;
    std::vector<double> associations;
    std::vector<double> sense_x;
    std::vector<double> sense_y;

    double lidar[200];

};


class ParticleFilter
{
public:
    ParticleFilter();
    ParticleFilter(int numberOfParticles)
        :numberOfParticles(numberOfParticles) {}


    std::vector<Particle> particles;

    void drawParticles(cv::Mat &map);

    void drawLidarData(cv::Mat &map);

    void lidarParticles(cv::Mat &map);

    void drawLidarParticles(cv::Mat &map);

    void robotLidar(Particle &robot, cv::Mat &map);

    void drawRobotParticle(Particle &robot, cv::Mat &map);

    void drawLidarRobot(Particle &robot, cv::Mat &map);

    void moveRobot(double delta_timestep, double stdPos, double velocity, double orientation_rate, Particle &robot);


    bool particleFilterInitialized(){ return particleFilterInit; }
    std::vector<double> getParticleWeights(){ return weights; }

    // Function to run the particle filter on map. Implement as the last thing if its possible
    void runParticleFilter(int map);

    // Function for particle filter
    void initializeParticleFilter(double x, double y, double orientation, double std[], cv::Mat &map);

    void prediction(double delta_timestep, double stdPos, double velocity, double orientation_rate, Particle robot);

    void associateData(const std::vector<LandmarkObservation> &predicted, std::vector<LandmarkObservation> &observations);

    void associateParticlesWithRobot(Particle &robot);

    void normalizeWeights(std::vector<double> &weights);

    void updateWeights(double lidar_range, double stdLandmark[], const std::vector<LandmarkObservation> &observations, const Map &map_landmarks);

    void printParticles(Particle particles);

    void resampleParticles(int numberOfResample, double stddivPosition);

    void setAssociations(Particle& particle, std::vector<double> new_associations, std::vector<double> new_sense_x, std::vector<double> new_sense_y);

    std::vector<double> getAssociations(Particle best) { return best.associations; }
    std::vector<double> getSenseX(Particle best) { return best.sense_x; }
    std::vector<double> getSenseY(Particle best) {return best.sense_y; }


private:


    // True random numbers
    std::random_device rd;
    // Pseudo random numbers
    std::default_random_engine dre;


    double lidarMaxRange = 10.0;
    double lidarFov = 270.0;
    double noPoints = 200.0;
    double angle_increment = (lidarFov / noPoints) * (3.14 / 180.0);

    int numberOfParticles = 0;
    bool particleFilterInit = false;

    std::vector<double> weights;
    std::vector<double> normalizedWeights;

};

#endif // PARTICLEFILTER_H
