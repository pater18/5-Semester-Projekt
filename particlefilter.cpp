#include "particlefilter.h"


ParticleFilter::ParticleFilter()
{
    std::cout << "Specify the Number of Particles in Contructor!" << std::endl;

}


void ParticleFilter::initializeParticleFilter(double x, double y, double orientation, double std[], cv::Mat &map){


    std::uniform_real_distribution<double> rand_x(x, map.cols);
    std::uniform_real_distribution<double> rand_y(y, map.rows);
    std::uniform_real_distribution<double> rand_orien(0.0, 360.0);


    for(int i = 0; i < numberOfParticles; i++){

        Particle particle;
        particle.x = rand_x(rd);
        particle.y = rand_y(rd);
        particle.orientation = rand_orien(rd);
        particle.id = i;
        particle.weight = 1.0;
        weights.push_back(particle.weight);

        particles.push_back(particle);

    }

    particleFilterInit = true;

}


void ParticleFilter::lidarParticles(cv::Mat &map){


    for(int par = 0; par < particles.size(); par++){

        for(int point = 0; point < noPoints; point++) {
          // angle for the lidar point
          float angle = particles[par].orientation * (3.14 / 180.0) - 100 * angle_increment + point * angle_increment;

          //float angle = i * angle_increment;

          cv::Point2f endPoint(particles[par].x + lidarMaxRange * std::cos(angle), particles[par].y + lidarMaxRange * std::sin(angle));

          cv::Point particlePoint = {particles[par].x, particles[par].y};

          cv::LineIterator lineIt(map, particlePoint, endPoint);

          for(int j = 0; j < lineIt.count; j++, ++lineIt){
              if(map.at<cv::Vec3b>(lineIt.pos())[0] != 255 && map.at<cv::Vec3b>(lineIt.pos())[1] != 255){
                  particles[par].lidar[point] = cv::norm(particlePoint - lineIt.pos());
                  break;
              } else {
                   particles[par].lidar[point] = lidarMaxRange;
              }
          }


         //cv::line(map, cv::Point2f(particles[par].x, particles[par].y), endPoint, cv::Scalar(0,0,255));
        }
    }

}


void ParticleFilter::drawLidarParticles(cv::Mat &map){


    for(int par = 0; par < particles.size(); par++){
        for(int point = 0; point < noPoints; point++){

            float angle = particles[par].orientation * (3.14 / 180.0) - 100 * angle_increment + point * angle_increment;;

            cv::Point2f endPoint(particles[par].x + particles[par].lidar[point] * std::cos(angle), particles[par].y + particles[par].lidar[point] * std::sin(angle));

            cv::line(map, cv::Point2f(particles[par].x, particles[par].y), endPoint, cv::Scalar(0,0,255));

        }
    }

}




void ParticleFilter::drawParticles(cv::Mat &map){

    for(auto &par : particles){
        cv::circle(map, cv::Point(par.x, par.y), 1, cv::Scalar(255,0,0));
    }
}


void ParticleFilter::drawLidarData(cv::Mat &map){

    double lidarMaxRange = 10.0;
    double lidarFov = 270.0;
    double noPoints = 200.0;
    double angle_increment = (lidarFov / noPoints) * (3.14 / 180.0);

}


// orientation_rate = delta direction / delta time step - updates somewhere else
void ParticleFilter::prediction(double delta_timestep, double stdPos, double velocity, double orientation_rate){


    for(auto &particle : particles){

        double x = particle.x;
        double y = particle.y;
        double orien = particle.orientation;

        // Odometry
        x = x + (velocity / orientation_rate) * ( sin(orien + delta_timestep * orientation_rate) - sin(orien) );
        y = y + (velocity / orientation_rate) * ( cos(orien) - cos(orien + delta_timestep * orientation_rate) );
        orien = orien + delta_timestep * orientation_rate;

        // Noise from odometry
        std::normal_distribution<double> noise_x(x, stdPos);
        std::normal_distribution<double> noise_y(y, stdPos);
        std::normal_distribution<double> noise_orien(orien, stdPos);

        particle.x = noise_x(rd);
        particle.y = noise_y(rd);
        particle.orientation = noise_orien(rd);

    }
}


void ParticleFilter::associateData(const std::vector<LandmarkObservation>& predictions, std::vector<LandmarkObservation>& observations){

    for(size_t obs = 0; obs < observations.size(); obs++){

        double distance = 0.0;
        double min_distance = 100.0;

        for(size_t pre = 0; pre < predictions.size(); pre++){

            distance = std::sqrt(std::pow((observations[obs].x - predictions[pre].x), 2) + std::pow((observations[obs].y - predictions[pre].y), 2));

            if(distance < min_distance){
                min_distance = distance;
                observations[obs].id = predictions[pre].id;
            }
        }
    }
}


void ParticleFilter::updateWeights(double lidar_range, double stdLandmark[], const std::vector<LandmarkObservation> &observations, const Map &map_landmarks){

    for(auto &p : particles){


        // 1. Match map landmarks from particles and observation coordinates from lidar




        // 2. Associate the landmarks and observations




        // 3. Update the weights









    }

}




void ParticleFilter::resampleParticles(){

    std::discrete_distribution<int> resampleDist(weights.begin(), weights.end());

    // Adaptive monte carlo - decrease number of particles for each resample or once in a while
    for(size_t i = 0; i < particles.size(); i++){
        particles[i] = particles[resampleDist(rd)];
    }

}


void ParticleFilter::setAssociations(Particle& particle, std::vector<double> new_associations, std::vector<double> new_sense_x, std::vector<double> new_sense_y){

    particle.associations.clear();
    particle.sense_x.clear();
    particle.sense_y.clear();

    particle.associations = new_associations;
    particle.sense_x = new_sense_x;
    particle.sense_y = new_sense_y;

}
