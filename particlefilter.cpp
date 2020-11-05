#include "particlefilter.h"

ParticleFilter::ParticleFilter()
{
    std::cout << "Specify the Number of Particles in Contructor!" << std::endl;

}


void ParticleFilter::initializeParticleFilter(double x, double y, double orientation, double std[]){


}


void ParticleFilter::prediction(double delta_timestep, double stdPos, double velocity, double orientation_rate){


}


void ParticleFilter::associateData(const std::vector<LandmarkObservation>& predicted, std::vector<LandmarkObservation>& observations){


}


void ParticleFilter::updateWeights(double lidar_range, double stdLandmark[], const std::vector<LandmarkObservation> &observations, const Map &map_landmarks){


}


void ParticleFilter::printParticles(Particle particles){


}


void ParticleFilter::resampleParticles(){


}


void ParticleFilter::setAssociations(Particle& particle, std::vector<double> new_associations, std::vector<double> new_sense_x, std::vector<double> new_sense_y){

    particle.associations.clear();
    particle.sense_x.clear();
    particle.sense_y.clear();

    particle.associations = new_associations;
    particle.sense_x = new_sense_x;
    particle.sense_y = new_sense_y;

}
