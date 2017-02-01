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
    if(argc != 2) {
        std::cerr << "USAGE: " << argv[0] << " <imagefile>" << std::endl;
        exit(EXIT_FAILURE);
    }

    // try to open image file
    cv::Mat image = cv::imread(argv[1]);
    if(!image.data) {
        std::cerr << "Invalid image file provided." << std::endl;
        exit(EXIT_FAILURE);
    }

    // ok we good now
    // now get hsv version of image
    // i'll tell you what - i just googled hsv
    // and got herpes...
    // anyway here we go
    cv::Mat hsvImage = bgr2hsv(image);

    // call our function that finds a dominant colour
    // it speaks hsv so be nice to it
    cv::Vec3b d_colour = find_dominant_colour(hsvImage);

    // display things so it looks really good
    display_image(argv[1], image);

    // find dominant pixels
    cv::Mat dominant_map = detect_dominant(hsvImage, d_colour);
    dominant_map = hsv2bgr(dominant_map);
    display_image("Dominant zones", dominant_map);

    // and the dominant colour
    /*cv::Mat dominant(200, 200, CV_8UC3, cv::Scalar(d_colour[0], d_colour[1], d_colour[2]));
    dominant = hsv2bgr(dominant);
    display_image("Dominant colour", dominant);*/

    // stop here
	cv::waitKey(0);

    return 0;
}
