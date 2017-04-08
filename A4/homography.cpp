#include "homography.hpp"

#include "match.hpp"

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
