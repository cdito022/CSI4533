#include <cmath>
#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

#define DEGREES_PER_BIN 15
#define NUM_BINS 12

void display_image(std::string name, cv::Mat image) {
    cv::namedWindow(name);
    cv::imshow(name, image);
}

// USAGE: ./dominant <imagefile>
int main(int argc, char* argv[]) {
    // some validation
    if(argc != 2) {
        std::cerr << "USAGE: " << argv[0] << " <imagefile>" << std::endl;
        exit(EXIT_FAILURE);
    }

    // try to open image file
    cv::Mat image = cv::imread(argv[1]);
    if(!image.data) {
        std::cerr << "Invalid image file provided." << std::endl;
        exit(EXIT_FAILURE);
    }

    // ok we good now
    // now get hsv version of image
    // i'll tell you what - i just googled hsv
    // and got herpes...
    // anyway here we go
    cv::Mat hsvImage;
    cv::cvtColor(image, hsvImage, CV_BGR2HSV);

    // the bins we count
    // we use 12 different colours
    // that's like 30 degrees per colour on the hsv thingy
    // for opencv purposes it's actually only 15 degrees
    float bins[NUM_BINS]{};

    for(int row = 0; row < hsvImage.rows; ++row) {
		cv::Vec3b* current = hsvImage.ptr<cv::Vec3b>(row);

        for(int col = 0; col < hsvImage.cols; ++col) {
            // add 7 so everything red won't fall over zero
            int bin = ((current[col][0] + DEGREES_PER_BIN / 2) % 180) / DEGREES_PER_BIN;
            bins[bin]+= (current[col][1] / 255.0) * (current[col][2] / 255.0);
        }
	}

    // find the max bin
    int maxbin = 0;
    for(int i = 0; i < NUM_BINS; ++i) {
        if(bins[i] > bins[maxbin]) {
            maxbin = i;
        }
        std::cout << "Bin " << i << ": " << bins[i] << std::endl;
    }

    std::cout << "The dominant colour is in bin " << maxbin << " with a value of " << bins[maxbin] << std::endl;

    // display things so it looks really good
    display_image(argv[1], image);

    // and the dominant colour
    cv::Mat hsvDominant(200, 200, CV_8UC3, cv::Scalar(maxbin*DEGREES_PER_BIN,255,255));
    cv::Mat dominant;
    cv::cvtColor(hsvDominant, dominant, CV_HSV2BGR);

    display_image("Dominant colour", dominant);

    // stop here
	cv::waitKey(0);

    return 0;
}
