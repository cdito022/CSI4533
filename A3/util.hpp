#ifndef UTIL_HPP
#define UTIL_HPP

#include <opencv2/core/core.hpp>

bool load_images(int argc, char* argv[], cv::Mat& target, cv::Mat& image);

void display_image(std::string name, cv::Mat image);

#endif
