#include "pathplanning.h"

pathPlanning::pathPlanning()
{

    // loading image
    cv::Mat image = cv::imread("floor_plan.png");
    if (image.empty()){
        std::cerr << "Could not load image";
    }

    // Grayscale image to get one channel
    cv::cvtColor(image, image, cv::COLOR_RGB2GRAY);

    size_t cols = image.cols;
    size_t rows = image.rows;

    // Assining each pixel a value increasing from a wall from left to right
    int index = 2;
    for (size_t i = 0; i < rows; i++){
        std::vector<int> tempRows; // Temporary vector to contain values of each row.
        for (size_t j = 0; j < cols; j++) {
            if ((int)image.at<uchar>(i, j) != black){
                tempRows.push_back(index++);
            } else if ((int)image.at<uchar>(i, j) == black){
                tempRows.push_back(0);
                index = 2;
            }
        }
        brushValues.push_back(tempRows);
    }

    //Pixel values from right to left
    index = 2;
    for (size_t i = 0; i < rows; i++){
        for (size_t j = 0; j < cols; j++) {
            if (brushValues[i][cols - 1 - j] != 0 && brushValues[i][cols - 1 - j] > index){
                brushValues[i][cols - 1 - j] = index++;
            } else if (brushValues[i][cols - 1 - j] == 0){
                index = 2;
            }
        }
    }

    //Pixel values from top to bottom
    index = 2;
    for (size_t j = 0; j < cols; j++){
        for (size_t i = 0; i < rows; i++) {
            if (brushValues[i][j] != 0 && brushValues[i][j] > index){
                brushValues[i][j] = index++;
            } else if (brushValues[i][j] == 0){
                index = 2;
            }
        }
    }

    //Pixel values from bottom to top
    index = 2;
    for (size_t j = 0; j < cols; j++){
        for (size_t i = 0; i < rows; i++) {
            if (brushValues[rows - 1 - i][j] != 0 && brushValues[rows - 1 - i][j] > index){
                brushValues[rows - 1 - i][j] = index++;
            } else if (brushValues[rows - 1 - i][j] == 0){
                index = 2;
            }
        }
    }


    // Shows the outut of the brushfire before correcting
    for (size_t i = 0; i < brushValues.size(); i++){
        for (size_t j = 0; j < brushValues[i].size(); j++){
            std::cout << std::setw(3) << brushValues[i][j] ;
        }
        std::cout << "next line "<< std::endl;
    }
    std::cout << std::endl;

    // Correction of numbers so they only jump one up or down in each direction
    int value = 0;
    while(value < 15){

        for (size_t j = 0; j < cols; j++){
            for (size_t i = 0; i < rows; i++) {
                // Checks if the neighbour right is 2 or more values from the current pixel value
                if (brushValues[i][j] != 0 && brushValues[i][j] != 2){
                    if (abs(brushValues[i][j] - brushValues[i + 1][j]) >= 2 ){


                        if (brushValues[i][j] - brushValues[i + 1][j] <= -2 ){
                            brushValues[i][j]++;
                        } else if (brushValues[i][j] - brushValues[i + 1][j] >= 2 ) {
                            brushValues[i][j]--;
                        }
                    }

                    // Checks neighbour left
                    if (abs(brushValues[i][j] - brushValues[i - 1][j]) >= 2 ){


                        if (brushValues[i][j] - brushValues[i - 1][j] <= -2 ){
                            brushValues[i][j]++;
                        } else if (brushValues[i][j] - brushValues[i - 1][j] >= 2 ){
                            brushValues[i][j]--;
                        }
                    }

                    // Checks neighbour down
                    if (abs(brushValues[i][j] - brushValues[i][j + 1]) >= 2 ){

                        if (brushValues[i][j] - brushValues[i][j + 1] <= -2 ){
                            brushValues[i][j]++;
                        } else if (brushValues[i][j] - brushValues[i][j + 1] >= 2 ){
                            brushValues[i][j]--;
                        }
                    }

                    // Checks neighbour up
                    if (abs(brushValues[i][j] - brushValues[i][j - 1]) >= 2 ){

                        if (brushValues[i][j] - brushValues[i][j - 1] <= -2 ){
                            brushValues[i][j]++;
                        } else if (brushValues[i][j] - brushValues[i][j - 1] >= 2 ){
                            brushValues[i][j]--;
                        }
                    }
                }
            }
        }
        value++;
    }

    //Show the result of the brushfire algorithme
    for (size_t i = 0; i < brushValues.size(); i++){
        for (size_t j = 0; j < brushValues[i].size(); j++){
            std::cout << std::setw(3) << brushValues[i][j] ;
        }
        std::cout << "next line "<< std::endl;
    }

    const double divider = 255 / 15; // 15 is choosen because it is the highest value in the grid

    // Create a grayscale image of the brushfire algoritme
    cv::Mat Brushfire (image.rows, image.cols, CV_8UC1);
    for (size_t i = 0; i < brushValues.size(); i++){
        for (size_t j = 0; j < brushValues[i].size(); j++){
            Brushfire.at<uchar>(i, j) = divider * brushValues[i][j];
        }
    }


    cv::Mat BigBrush, originalScaled;
    cv::Size size(300, 300);

    cv::resize(Brushfire, BigBrush, size);
    cv::resize(image, originalScaled, size);
    std::cout << std::endl;

    cv::namedWindow("Brushfire");

    cv::imshow("Scaled original map", originalScaled);
    cv::imshow("Brushfire", BigBrush);
    cv::imwrite("brushfire.png", Brushfire);
    cv::waitKey(0);

}
