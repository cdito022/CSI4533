#include "transformation.hpp"

#include <vector>

#include <opencv2/core.hpp>

std::vector<cv::Point2f> transform_corners(cv::Mat image, cv::Mat homography) {
	std::vector<cv::Point2f> corners;

	corners.push_back(cv::Point2f(0, 0));
	corners.push_back(cv::Point2f(image.cols - 1, 0));
	corners.push_back(cv::Point2f(image.cols - 1, image.rows - 1));
	corners.push_back(cv::Point2f(0, image.rows - 1));

	cv::perspectiveTransform(corners, corners, homography);

	return corners;
}
