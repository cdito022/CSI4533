#include <cmath>
#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "dominant.hpp"
#include "util.hpp"

#define DEGREES_PER_BIN 15
#define NUM_BINS 12

void display_image(std::string name, cv::Mat image) {
    cv::namedWindow(name);
    cv::imshow(name, image);
}

// USAGE: ./dominant <imagefile>
int main(int argc, char* argv[]) {
    // some validation
    if(argc != 3) {
        std::cerr << "USAGE: " << argv[0] << " <targetfile> <imagefile>" << std::endl;
        exit(EXIT_FAILURE);
    }

    // try to open target file
    cv::Mat target = cv::imread(argv[1]);
    if(!target.data) {
        std::cerr << "Invalid target file provided." << std::endl;
        exit(EXIT_FAILURE);
    }

    // try to open image file
    cv::Mat image = cv::imread(argv[2]);
    if(!image.data) {
        std::cerr << "Invalid image file provided." << std::endl;
        exit(EXIT_FAILURE);
    }

    // display the images we got
    display_image("Target", target);
    display_image("Image to search", image);

    // now get hsv version of target
    // and its dominant colour
    // i'll tell you what - i just googled hsv
    // and got herpes...
    // anyway here we go
    cv::Vec3b dominant = find_dominant_colour(bgr2hsv(target));

    // call our function that finds dominant pixels
    // it speaks hsv so be nice to it
    cv::Mat dominant_map = hsv2bgr(detect_dominant(bgr2hsv(image), dominant));
    display_image("Dominant zones", dominant_map);

    // stop here
	cv::waitKey(0);

    return 0;
}
