#ifndef HOMOGRAPHY_HPP
#define HOMOGRAPHY_HPP

#include <vector>

#include <opencv2/core.hpp>

// homography contains the homography between an image
// and the image at position `with` in the frames array
struct homography_descriptor {
	int with;
	cv::Mat matrix;
};

// find_best_homography tries to find an acceptable homography
// between the image at `pos` and the earliest image in the array
struct homography_descriptor find_best_homography(std::vector<cv::Mat>& frames, int pos);

// find_homography tries to find an homography between images A and B
cv::Mat find_homography(cv::Mat A, cv::Mat B);

// chain_homographies tries to establish a relationship
// between all frames and the first frame
void chain_homographies(std::vector<homography_descriptor>& homographies);

void display_homography_matrix(cv::Mat homography);

#endif
