#include "match.hpp"

#include <vector>

#include <opencv2/calib3d.hpp>
#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>

std::vector<cv::KeyPoint> detect_keypoints(cv::Mat& image) {
	std::vector<cv::KeyPoint> keypoints;

	auto detector = cv::BRISK::create(100);
	detector->detect(image, keypoints);

	return keypoints;
}

cv::Mat compute_descriptors(cv::Mat& image, std::vector<cv::KeyPoint>& keypoints) {
	cv::Mat descriptors;

	auto descriptor = cv::BRISK::create();
	descriptor->compute(image, keypoints, descriptors);

	return descriptors;
}

std::vector<cv::DMatch> match_descriptors(cv::Mat& descA, cv::Mat& descB) {
	std::vector<cv::DMatch> matches;

	cv::BFMatcher matcher(cv::NORM_L2, true);
	matcher.match(descA, descB, matches);

	return matches;
}

cv::Mat ransac_filter_matches(std::vector<cv::KeyPoint>& tgtKeypoints,
							  std::vector<cv::KeyPoint>& imgKeypoints,
							  std::vector<cv::DMatch>& matches) {

	// convert keypoints into regular points
	std::vector<cv::Point2f> tgtPoints, imgPoints;
	tgtPoints.reserve(matches.size());
	imgPoints.reserve(matches.size());

	for(auto it = matches.begin(); it != matches.end(); ++it) {
		tgtPoints.push_back(tgtKeypoints[it->queryIdx].pt);
		imgPoints.push_back(imgKeypoints[it->trainIdx].pt);
	}

	// target's keypoints
	// TODO look into using vector<bool>
	std::vector<uchar> inliers(matches.size(), 0);
	cv::Mat homography = cv::findHomography(tgtPoints, imgPoints, inliers, cv::RHO, 1.0);

	int dst = 0, src = 0;
	while(src < matches.size()) {
		// move src to an inlier
		if(inliers[src] == 0) {
			++src;
		}
		else {
			matches[dst++] = matches[src++];
		}
	}

	// remove extra matches
	// the vector is returned as referenced array
	matches.resize(dst);

	return homography;
}
