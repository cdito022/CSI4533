#include <iostream>
#include <string>

#include "match.hpp"
#include "pyramid.hpp"
#include "util.hpp"

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

/* TARGET DETECTION
	Target = picture of truck
	Input image = scene containing truck

	1. Pyramid of target images
	2. Find interest points in each image of the pyramid
	3. Find interest points in input image
*/

int main(int argc, char* argv[]) {
	cv::Mat target, image;
	if(!load_images(argc, argv, target, image)) {
		exit(EXIT_FAILURE);
	}

	auto targetKeypoints = detect_keypoints(target);
	auto imageKeypoints = detect_keypoints(image);

	auto targetDescriptors = compute_descriptors(target, targetKeypoints);
	auto imageDescriptors = compute_descriptors(image, imageKeypoints);

	auto matches = match_descriptors(targetDescriptors, imageDescriptors);

	std::cout << "The target has " << targetDescriptors.size() << " descriptors" << std::endl;
	std::cout << "The image has " << imageDescriptors.size() << " descriptors" << std::endl;
	std::cout << "There were " << matches.size() << " matches found" << std::endl;

	display_image("target descriptors", target);
	display_image("image descriptors", image);

	cv::waitKey(0);

	return 0;
}
