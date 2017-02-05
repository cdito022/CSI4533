#ifndef MONOCHROMATIC_H
#define MONOCHROMATIC_H

#include <opencv2/core/core.hpp>

// erodes an image using mask size h*v
cv::Mat erode(cv::Mat image);

cv::Point most_surrounded_pixel(cv::Mat image, int dist);

#endif
