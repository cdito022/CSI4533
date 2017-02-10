#include <cmath>
#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "dominant.hpp"
#include "monochromatic.hpp"
#include "util.hpp"


// USAGE: ./find <imagefile>
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
    cv::Vec3b dominant_colour = find_dominant_colour(bgr2hsv(target));

    // call our function that finds dominant pixels
    // it speaks hsv (returns rgb) so be nice to it
    cv::Mat dominant_map = bgr2hsv(image);

    dominant_map = detect_dominant(dominant_map, dominant_colour, 100, 100);
    display_image("Dominant zones, pre-erode", dominant_map);

    dominant_map = erode(dominant_map);
    display_image("Dominant zones, post-erode", dominant_map);

    // find most surrounded pixel and draw box around
    cv::Point msp = most_surrounded_pixel(dominant_map, 10);
    draw_box(image, cv::Vec3b(0,0,255), msp, 20, 20);
    display_image("Estimated object position", image);

    // stop here
	cv::waitKey(0);

    return 0;
}
