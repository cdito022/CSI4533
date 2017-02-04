#include "monochromatic.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

cv::Mat erode(cv::Mat image) {
	cv::Mat final;
	cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(4,4), cv::Point(0,0));

	cv::erode(image, final, kernel);
	return final;
}
