#include "detection.hpp"

#include <iostream>

#include "match.hpp"
#include "pyramid.hpp"
#include "util.hpp"

#include <opencv2/calib3d.hpp>
#include <opencv2/core.hpp>

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

std::vector<cv::Point2f> find_best_match(cv::Mat& target, cv::Mat& image) {
	// input image
	auto keypoints = detect_keypoints(image);
	auto descriptors = compute_descriptors(image, keypoints); // we match this

	// target image
	auto pyramid = make_pyramid(target, 0.9, 8);

	// best values
	cv::Mat bestHomography;
	cv::Size bestSize;
	int maxMatches = 0;

	// loop through the pyramid
	for(auto it = pyramid.begin(); it != pyramid.end(); ++it) {
		// keypoints and descriptors
		auto tgtKeypoints = detect_keypoints(*it);
		auto tgtDescriptors = compute_descriptors(*it, tgtKeypoints);

		// matching, filtering with ransac
		auto matches = match_descriptors(tgtDescriptors, descriptors);
		auto homography = ransac_filter_matches(tgtKeypoints, keypoints, matches);
		// matches now contains only the inliers

		// update best values
		if(matches.size() > maxMatches) {
			maxMatches = matches.size();
			bestHomography = homography;
			bestSize = it->size();
		}
	}

	// this will hold the corners of the target in the input image
	std::vector<cv::Point2f> transformedCorners;

	// we like 8 good inliers
	if(maxMatches > 8) {
		std::vector<cv::Point2f> corners;

		corners.push_back(cv::Point2f(0, 0));
		corners.push_back(cv::Point2f(bestSize.width - 1, 0));
		corners.push_back(cv::Point2f(bestSize.width - 1, bestSize.height - 1));
		corners.push_back(cv::Point2f(0, bestSize.height - 1));

		// transform the corners using the best homography found
		cv::perspectiveTransform(corners, transformedCorners, bestHomography);
	}

	// if empty, there was no good match
	return transformedCorners;
}
