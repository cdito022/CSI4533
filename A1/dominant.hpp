#ifndef DOMINANT_HPP
#define DOMINANT_HPP

#include <opencv2/core/core.hpp>

#define NUM_COLOURS     12
#define DEGREES_PER_BIN 15

int reduce_colour(int hue);

cv::Vec3b find_dominant_colour(cv::Mat image);

cv::Mat detect_dominant(cv::Mat image, cv::Vec3b colour);

#endif
