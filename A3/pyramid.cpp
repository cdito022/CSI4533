#include "pyramid.hpp"

#include <cmath>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

std::vector<cv::Mat> make_pyramid(cv::Mat& original, double factor, int number) {
	std::vector<cv::Mat> pyramid;
	pyramid.reserve(number);
	pyramid.push_back(original.clone());

	for(int i = 1; i < number; ++i) {
		cv::Mat resized;
		cv::resize(original, resized, cv::Size(0,0), std::pow(factor, i), std::pow(factor, i));
		pyramid.push_back(resized);
	}

	return pyramid;
}
