#include <iostream>
#include <string>

#include "detection.hpp"
#include "match.hpp"
#include "pyramid.hpp"
#include "util.hpp"

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

int main(int argc, char* argv[]) {
	cv::Mat target, image;
	if(!load_images(argc, argv, target, image)) {
		exit(EXIT_FAILURE);
	}

	auto corners = find_best_match(target, image);
	if(corners.size() == 0) {
		std::cerr << "Unable to find target in image." << std::endl;
		exit(EXIT_FAILURE);
	}

	draw_borders(image, corners, cv::Scalar(0,255,0));
	display_image("Target", target);
	display_image("Image", image);

	cv::waitKey(0);

	return 0;
}
