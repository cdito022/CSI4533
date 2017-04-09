#include "homography.hpp"

#include <cstdio>

#include "match.hpp"
#include "util.hpp"

#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/highgui.hpp>

struct homography_descriptor find_best_homography(std::vector<cv::Mat>& frames, int pos) {
	cv::Mat frame = frames[pos];
	struct homography_descriptor homography;
	homography.with = -1; // default

	for(int i = 0; i < pos; ++i) {
		homography.matrix = find_homography(frames[i], frame);

		if(homography.matrix.data != nullptr) {
			homography.with = i;
			return homography;
		}
	}

	return homography;
}

cv::Mat find_homography(cv::Mat A, cv::Mat B) {
	// do stuff for image A
	auto keypointsA = detect_keypoints(A);
	auto descriptorsA = compute_descriptors(A, keypointsA);

	// do the same stuff for image B
	auto keypointsB = detect_keypoints(B);
	auto descriptorsB = compute_descriptors(B, keypointsB);

	// match and ransac
	auto matches = match_descriptors(descriptorsA, descriptorsB);
	auto homography = ransac_filter_matches(keypointsA, keypointsB, matches);

	// test for enough good matches
	if(matches.size() > 8) {
		return homography;
	}
	// or return empty image
	else {
		return cv::Mat();
	}
}

void chain_homographies(std::vector<homography_descriptor>& homographies) {
	for(int i = 0; i < homographies.size(); ++i) {
		auto& homography = homographies[i];

		while(homography.with > 0) {
			// chain homographies
			homography.matrix = homographies[homography.with].matrix * homography.matrix;
			homography.with = homographies[homography.with].with;
		}
	}
}

void display_homography_matrix(cv::Mat homography) {
	for(int row = 0; row < homography.rows; ++row) {
		for(int col = 0; col < homography.cols; ++col) {
			printf("%f ", homography.at<double>(row, col));
		}
		printf("\n");
	}
}
