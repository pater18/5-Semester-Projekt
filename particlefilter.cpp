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
              if(map.at<cv::Vec3b>(lineIt.pos())[0] == 0 && map.at<cv::Vec3b>(lineIt.pos())[1] == 0){
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


void ParticleFilter::robotLidar(Particle &robot, cv::Mat &map){


    for(int point = 0; point < noPoints; point++) {
      // angle for the lidar point
      float angle = robot.orientation * (3.14 / 180.0) - 100 * angle_increment + point * angle_increment;

      //float angle = i * angle_increment;

      cv::Point2f endPoint(robot.x + lidarMaxRange * std::cos(angle), robot.y + lidarMaxRange * std::sin(angle));

      cv::Point particlePoint = {robot.x, robot.y};

      cv::LineIterator lineIt(map, particlePoint, endPoint);

      for(int j = 0; j < lineIt.count; j++, ++lineIt){
          if(map.at<cv::Vec3b>(lineIt.pos())[0] == 0 && map.at<cv::Vec3b>(lineIt.pos())[1] == 0){
              robot.lidar[point] = cv::norm(particlePoint - lineIt.pos());
              break;
          } else {
               robot.lidar[point] = lidarMaxRange;
          }
      }

     //cv::line(map, cv::Point2f(particles[par].x, particles[par].y), endPoint, cv::Scalar(0,0,255));

    }
}


void ParticleFilter::drawRobotParticle(Particle &robot, cv::Mat &map){

    cv::circle(map, cv::Point(robot.x, robot.y), 1, cv::Scalar(255,0,0));

    for(int point = 0; point < noPoints; point++){

        float angle = robot.orientation * (3.14 / 180.0) - 100 * angle_increment + point * angle_increment;;

        cv::Point2f endPoint(robot.x + robot.lidar[point] * std::cos(angle), robot.y + robot.lidar[point] * std::sin(angle));

        cv::line(map, cv::Point2f(robot.x, robot.y), endPoint, cv::Scalar(255,0,0));

    }


}



void ParticleFilter::drawLidarParticles(cv::Mat &map){

    for(int par = 0; par < particles.size(); par++){
        for(int point = 0; point < noPoints; point++){

            float angle = particles[par].orientation * (3.14 / 180.0) - 100 * angle_increment + point * angle_increment;;

            cv::Point2f endPoint(particles[par].x + particles[par].lidar[point] * std::cos(angle), particles[par].y + particles[par].lidar[point] * std::sin(angle));

            cv::line(map, cv::Point2f(particles[par].x, particles[par].y), endPoint, cv::Scalar(0,255,0));

        }
    }
}


void ParticleFilter::drawParticles(cv::Mat &map){

    for(auto &par : particles){
        cv::circle(map, cv::Point(par.x, par.y), 1, cv::Scalar(255,0,0));
    }
}



// orientation_rate = delta direction / delta time step - updates somewhere else
void ParticleFilter::prediction(double delta_timestep, double stdPos, double velocity, double orientation_rate){


    for(auto &particle : particles){

        double x = particle.x;
        double y = particle.y;
        double orien = particle.orientation;

        std::cout << "x1: " << x << " y1: "<< y << " orien1: " << orien << std::endl;

        // Odometry

        if(orientation_rate == 0){
            x += velocity * delta_timestep * std::cos(orien);
            y += velocity * delta_timestep * std::sin(orien);
        } else {
            x += (velocity / orientation_rate) * ( sin(orien + delta_timestep * orientation_rate) - sin(orien) );
            y += (velocity / orientation_rate) * ( cos(orien) - cos(orien + delta_timestep * orientation_rate) );
            orien += delta_timestep * orientation_rate;
        }

        std::cout << "x: " << x << " y: "<< y << " orien: " << orien << std::endl;
//        particle.x = x;
//        particle.y = y;
//        particle.orientation = orien;

        // Noise from odometry
        std::normal_distribution<double> noise_x(x, stdPos);
        std::normal_distribution<double> noise_y(y, stdPos);
        std::normal_distribution<double> noise_orien(orien, stdPos);

        particle.x = noise_x(rd);
        particle.y = noise_y(rd);
        particle.orientation = noise_orien(rd);

        std::cout << "x2: " << x << " y2: "<< y << " orien2: " << orien << std::endl;

    }
}

void ParticleFilter::associateParticlesWithRobot(Particle &robot){

    double stddiv = 5.0;
    double a = (1 / (stddiv * std::sqrt(2*CV_PI)));

    int i = 0;
    for(auto &particle : particles){

        for(int point = 0; point < noPoints; point++){

            if(particle.lidar[point] == 0){
                particle.weight = 0;
            } else {
                particle.weight += a * std::exp(-(std::pow(particle.lidar[point] - robot.lidar[point], 2) / (2 * std::pow(stddiv, 2))));
            }

        }

        weights[i] = particle.weight;

        i++;
    }

    //normalizeWeights(weights);

    for(auto &we : weights){
        std::cout <<"weight: " << we << std::endl;
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


void ParticleFilter::normalizeWeights(std::vector<double> &weights){

    double maxElement = *std::max_element(weights.begin(), weights.end());
    double minElement = *std::min_element(weights.begin(), weights.end());


    double sumOfWeights = 0.0;

    for(auto &weight : weights){
        if(weight != 0){
            sumOfWeights += weight;
        }
    }

    for(int i = 0; i < weights.size(); i++){

       weights[i] = (weights[i] - minElement) / (maxElement - minElement);
//       if(sumOfWeights != 0){
//            weights[i] /= sumOfWeights;
//       }

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
