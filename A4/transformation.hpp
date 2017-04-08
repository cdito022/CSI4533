#ifndef TRANSFORMATION_HPP
#define TRANSFORMATION_HPP

#include <vector>

#include <opencv2/core.hpp>

std::vector<cv::Point2f> transform_corners(cv::Mat image, cv::Mat homography);

#endif
