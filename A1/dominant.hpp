#ifndef DOMINANT_HPP
#define DOMINANT_HPP

#include <opencv2/core/core.hpp>

#define NUM_COLOURS     15
#define DEGREES_PER_BIN 12

int reduce_colour(int hue);

cv::Vec3b find_dominant_colour(cv::Mat image);

cv::Mat detect_dominant(cv::Mat image, cv::Vec3b colour, int s_threshold, int v_threshold);

#endif
