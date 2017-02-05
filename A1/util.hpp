#ifndef UTIL_HPP
#define UTIL_HPP

#include <opencv2/core/core.hpp>

cv::Mat bgr2hsv(cv::Mat bgr);

cv::Mat hsv2bgr(cv::Mat hsv);

void display_image(std::string name, cv::Mat image);

void draw_box(cv::Mat image, cv::Vec3b colour, cv::Point center, int width, int height);

#endif
