#include "util.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>

cv::Mat bgr2hsv(cv::Mat bgr) {
    cv::Mat hsv;
    cv::cvtColor(bgr, hsv, CV_BGR2HSV);
    return hsv;
}

cv::Mat hsv2bgr(cv::Mat hsv) {
    cv::Mat bgr;
    cv::cvtColor(hsv, bgr, CV_HSV2BGR);
    return bgr;
}
