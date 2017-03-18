#include "match.hpp"

#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>

std::vector<cv::KeyPoint> detect_keypoints(cv::Mat& image) {
	std::vector<cv::KeyPoint> keypoints;

	auto detector = cv::BRISK::create(130);
	detector->detect(image, keypoints);

	return keypoints;
}

cv::Mat compute_descriptors(cv::Mat& image, std::vector<cv::KeyPoint>& keypoints) {
	cv::Mat descriptors;

	auto descriptor = cv::BRISK::create(130);
	descriptor->compute(image, keypoints, descriptors);

	return descriptors;
}

std::vector<cv::DMatch> match_descriptors(cv::Mat& descA, cv::Mat& descB) {
	std::vector<cv::DMatch> matches;

	cv::BFMatcher matcher(cv::NORM_L2, true);
	matcher.match(descA, descB, matches);

	return matches;
}
