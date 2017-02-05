#include "util.hpp"

#include <cstdio>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
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

void display_image(std::string name, cv::Mat image) {
    cv::namedWindow(name);
    cv::imshow(name, image);
}

// do exactly what the function name says
void draw_box(cv::Mat image, cv::Vec3b colour, cv::Point center, int width, int height) {
    // make zone
    int mincol = std::max(center.x - width / 2, 0);
    int minrow = std::max(center.y - height / 2, 0);
    int maxcol = std::min(center.x + width / 2, image.cols - 1);
    int maxrow = std::min(center.y + height / 2, image.rows - 1);

    cv::Mat zone = image(cv::Rect(mincol, minrow, maxcol-mincol, maxrow-minrow));

    // draw box
    for(int row = 0; row < zone.rows; ++row) {
		cv::Vec3b* current = zone.ptr<cv::Vec3b>(row);

        // top and bottom lines
        if(row == 0 || row == zone.rows - 1) {
            for(int col = 0; col < zone.cols; ++col) {
                current[col] = colour;
            }
        }

        // sides
        else {
            current[0] = colour;
            current[zone.cols - 1] = colour;
        }
	}
}
