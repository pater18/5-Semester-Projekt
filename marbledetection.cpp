#include "marbledetection.h"

MarbleDetection::MarbleDetection(){
}

void MarbleDetection::runMarbleDetection(){
    gazebo::transport::NodePtr nodeCamera(new gazebo::transport::Node());
    nodeCamera->Init();
    subCamera = nodeCamera->Subscribe("~/pioneer2dx/camera/link/camera/image", &MarbleDetection::cameraCallback, this);
}

void MarbleDetection::cameraCallback(ConstImageStampedPtr &msg) {


    static bool isInitialized;
    if(!isInitialized){
      isInitialized = true;
      cv::namedWindow("Camera Marble Detection", cv::WINDOW_NORMAL);
    }
    // Convert the msg image to a Mat object
    std::size_t width = msg->image().width();
    std::size_t height = msg->image().height();
    const char *data = msg->image().data().c_str();
    cv::Mat im(int(height), int(width), CV_8UC3, const_cast<char *>(data));

    cv::Mat img;
    cv::cvtColor(im, img, cv::COLOR_RGB2GRAY);
    // Reduce the noise to reduce/remove false detections
    //cv::medianBlur(img, img, 9);
    cv::GaussianBlur(img, img, cv::Size(9, 9), 2, 2 );
    //cv::fastNlMeansDenoising(img, img, 3);

    // Detect the marbles
    houghDetection(img, im, 20, 20);

    mutex.lock();
    //Display the image with marble detections
    cv::imshow("Camera Marble Detection", im);
    cv::moveWindow("Camera Marble Detection", 1500, 20);
    mutex.unlock();

}



void MarbleDetection::houghDetection(const cv::Mat &gray, const cv::Mat &imgOutput, int cannyValue, int accumulatorValue){

    // Vector that contains the output from the hough transformation function
    std::vector<cv::Vec3f> circles;
    // opencv built in function to detect circles in frame
    HoughCircles(gray, circles, cv::HOUGH_GRADIENT, 1, gray.rows/4, cannyValue, accumulatorValue, 0, 0 );

    //Set if marble is detected
    marbleDetected = !circles.empty();
    int closestCircleDetected = 0;



    // Get radius and outline of circles detected - We are interested in the closest and largest circle
    if(marbleDetected)
    {
      for( size_t i = 0; i < circles.size(); i++ )
      {
          cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
          // center of the circle
          int radius = cvRound(circles[i][2]);
          circle(imgOutput, center, 3, cv::Scalar(0,255,0), -1, 8, 0 );
          // outline of the cirle
          circle(imgOutput, center, radius, cv::Scalar(0,0,255), 3, 8, 0 );
          // Check if the circle is the closest
          if(radius > circles[closestCircleDetected][2])
          {
            closestCircleDetected = i;

          }
      }

      // Calculate angle to marble
      calculateAngleToMarble(circles, closestCircleDetected);
      if(marbleAngleHorizontal < -2){
         // std::cout << "Go Left" << std::endl;
      } else if(marbleAngleHorizontal > 2){
          //std::cout << "Go Right" << std::endl;
      }

      // Radius of closest circle

      int radiusClosest = circles[closestCircleDetected][2];

      if(radiusClosest > oldRadius){
        std::cout << "Radius: " << radiusClosest << std::endl;
        oldRadius = radiusClosest;
      }


      // Distance to marble when detected
      //calculateDistanceToMarbleFromCamera(radiusClosest);

    }

}



void MarbleDetection::calculateAngleToMarble(std::vector<cv::Vec3f> circles, int closestCircleDetected){

    const double horizontalResolution = 320;
    const double widthFromMid = horizontalResolution/2;
    const double horizontalFOV = 60;
    const double hFOVmiddle = horizontalFOV/2;

    int xCoordinateCircle = circles[closestCircleDetected][0];

    double displacementRatio = double(xCoordinateCircle) / widthFromMid;

    double marbleAngleHorizontal = (displacementRatio - 1) * hFOVmiddle;


    //std::cout << "Horizontal Angle: " << marbleAngleHorizontal << std::endl;


}


void MarbleDetection::calculateDistanceToMarbleFromCamera(int radiusClosest){

    marbleDistance = -0.0663 * radiusClosest + 6.43;
    std::cout << "distance: " << marbleDistance << std::endl;

}


void MarbleDetection::distanceToMarble(float distFront){

    if(marbleDetected){
        marbleDistance = distFront;
        std::cout << "distance: " << marbleDistance << std::endl;
    }
}

void MarbleDetection::distanceToMarbleOld(lidarSensor *lidar){

    if(marbleDetected){
        marbleDistance = lidar->getClosestObjectFront();
        std::cout << "distance: " << marbleDistance << std::endl;
    }
}

