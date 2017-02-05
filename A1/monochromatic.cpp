#include "monochromatic.hpp"

#include <cmath>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

// wrapper for OpenCV erode
cv::Mat erode(cv::Mat image) {
	cv::Mat final;
	cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(4,4), cv::Point(0,0));

	cv::erode(image, final, kernel);
	return final;
}

// how many white neighbors in the range
// return 0 if current pixel is black
int count_white_neighbors(cv::Mat& image, int& y, int& x, int& dist) {
	cv::Vec3b white(255, 255, 255);

	if(image.at<cv::Vec3b>(y,x) == white) {
		return 0;
	}

	int count = 0;

	for(int row = y - dist; row < y + dist; ++row) {
		for(int col = x - dist; col < x + dist; ++col) {
			if(image.at<cv::Vec3b>(row,col) == white) {
				++count;
			}
		}
	}

	return count;
}

// find the white with the most white around it
// nb. not a Donald Trump detector
cv::Point most_surrounded_pixel(cv::Mat image, int dist) {
	// make sure we have enough space to perform the algorithm
	dist = std::min( dist, (std::min(image.rows, image.cols) - 1) / 2 );

	cv::Point max_pixel(dist, dist);
	int max_value = 0;

	// go through image
	for(int row = dist; row < image.rows - dist; ++row) {
		cv::Vec3b* current = image.ptr<cv::Vec3b>(row);

        for(int col = dist; col < image.cols - dist; ++col) {
            int white_neighbors = count_white_neighbors(image, row, col, dist);
			if(white_neighbors > max_value) {
				max_pixel = cv::Point(col, row);
				max_value = white_neighbors;
			}
        }
	}

	return max_pixel;
}
