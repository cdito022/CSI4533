#ifndef UTIL_HPP
#define UTIL_HPP

#include <opencv2/core/core.hpp>

bool load_images(int argc, char* argv[], cv::Mat& target, cv::Mat& image);

void display_image(std::string name, cv::Mat image);

void draw_borders(cv::Mat& image, std::vector<cv::Point2f>& corners,
	cv::Scalar color = cv::Scalar(255, 255, 255), int thickness = 3);

#endif
