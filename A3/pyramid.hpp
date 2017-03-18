#ifndef PYRAMID_HPP
#define PYRAMID_HPP

#include <vector>

#include <opencv2/core.hpp>

std::vector<cv::Mat> make_pyramid(cv::Mat& original, double factor, int number);

#endif
