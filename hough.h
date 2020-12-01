#ifndef HOUGH_H
#define HOUGH_H

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;

double lidarDistanceMarble = 0;


double calculateDistanceToMarble(const double &radius, cv::Point &center){

    const double pixelToMM = 0.2645833333;
    double distance, distanceToMarble;

   // const double horiRes = 320;
    const double horiResMM = 84.66;

   // const double widthFromMid = horiRes/2;
    const double FOVdegHalf = 30.0;
    double FOVrad = FOVdegHalf * CV_PI/180.0; // 1.0472 rad
   // const double hFOVmiddle = FOV/2;

    //cout << "FOV: " << FOVrad << endl;

    double focalLenght = 0.0;

    focalLenght = horiResMM / (2.0 * tan(FOVrad));

    //cout << "focal length: " << focalLenght << endl;

//    double xR = (center.x + radius) / focalLenght;
//    double yR = center.y / focalLenght;
//    double xL = (center.x - radius) / focalLenght;
//    double yL = center.y / focalLenght;


//    double distanceIn3D = sqrt(pow(xR - xL, 2.0) + pow(yR - yL, 2.0));

//    distanceIn3D *= pixelToMM;

//    cout << "dist 3d: " << distanceIn3D << endl;


    double diameterDetectedMarble = radius / 2.0;

    diameterDetectedMarble *= pixelToMM;

    //cout << "dist 2d: " << diameterDetectedMarble << endl;

    distance = (focalLenght * horiResMM) / diameterDetectedMarble;


    distanceToMarble = (distance * pixelToMM) / 100.0;


   // distance = (distanceIn3D * focalLenght) / diameterDetectedMarble;


     cout << "dist: " << distanceToMarble << endl;
     cout << "Lidar dist: " << lidarDistanceMarble << endl;

    return distanceToMarble;

}



double houghDetection(cv::Mat &gray, cv::Mat &imgOutput, int cannyValue, int accumulatorValue){

    double distToMarble;

    // Vector that contains the output from the hough transformation function
    std::vector<cv::Vec3f> circles;

    // opencv built in function to detect circles in frame
    HoughCircles(gray, circles, cv::HOUGH_GRADIENT, 1, gray.rows/4, cannyValue, accumulatorValue, 0, 0 );

    //Set if marble is detected
    bool marbleDetected = false;
    marbleDetected = !circles.empty();

    int closestCircleDetected = 0;

    cv::cvtColor(gray, imgOutput, cv::COLOR_GRAY2RGB);

    // Get radius and outline of circles detected - We are interested in the closest and largest circle
    if(marbleDetected == true)
    {
      for( size_t i = 0; i < circles.size(); i++ )
      {
          cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
          // center of the circle
          double radius = cvRound(circles[i][2]);

          distToMarble = calculateDistanceToMarble(radius, center);

          //cout << "dist to marble: " << distToMarble << endl;

          circle(imgOutput, center, 3, cv::Scalar(0,0,255), -1, 8, 0 );
          // outline of the cirle
          circle(imgOutput, center, radius, cv::Scalar(0,255,0), 3, 8, 0 );
          // Check if the circle is the closest
          if(radius > circles[closestCircleDetected][2])
          {
            closestCircleDetected = i;

          }
      }

//      // Calculate angle to marble
//      calculateAngleToMarble(circles, closestCircleDetected);
//      if(marbleAngleHorizontal < -2){
//         // std::cout << "Go Left" << std::endl;
//      } else if(marbleAngleHorizontal > 2){
//          //std::cout << "Go Right" << std::endl;
//      }

//      // Radius of closest circle

//      int radiusClosest = circles[closestCircleDetected][2];

//      if(radiusClosest > oldRadius){
//        //std::cout << "Radius: " << radiusClosest << std::endl;
//        oldRadius = radiusClosest;
//      }


      // Distance to marble when detected
      //calculateDistanceToMarbleFromCamera(radiusClosest);

    }
    return distToMarble;
}






#endif // HOUGH_H
