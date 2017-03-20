#ifndef DETECTION_HPP
#define DETECTION_HPP

#include <vector>

#include <opencv2/core.hpp>

std::vector<cv::Point2f> find_best_match(cv::Mat& target, cv::Mat& image);

#endif
