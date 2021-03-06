#include "dominant.hpp"

#include <opencv2/core/core.hpp>

// reduce the hue (0-179) to one of the preset colours (0-11)
int reduce_colour(int hue) {
    // "+ DEGREES_PER_BIN / 2" degree 0 is exactly in the middle of red so we shift a bit
    return ((hue + DEGREES_PER_BIN / 2) % 180) / DEGREES_PER_BIN;
}

// convert from reduced colour to usable colour
int unreduce_colour(int colour) {
    return colour * DEGREES_PER_BIN;
}

// find the dominant colour in an image
// use an hsv image
cv::Vec3b find_dominant_colour(cv::Mat image) {
    // create bins
    // using double because numbers coming out of my 36 MP camera are HEAVY
    // we use 12 different colours
    // that's like 30 degrees per colour on the hsv thingy
    // for opencv purposes it's actually only 15 degrees
    double bins[NUM_COLOURS]{};

    for(int row = 0; row < image.rows; ++row) {
		cv::Vec3b* current = image.ptr<cv::Vec3b>(row);

        for(int col = 0; col < image.cols; ++col) {
            // reduce the colour
            int bin = reduce_colour(current[col][0]);
            bins[bin]+= (current[col][1] / 255.0) * (current[col][2] / 255.0);
        }
	}

    // find the max bin
    int maxbin = 0;
    for(int i = 0; i < NUM_COLOURS; ++i) {
        if(bins[i] > bins[maxbin]) {
            maxbin = i;
        }
    }

    cv::Vec3b dominant(unreduce_colour(maxbin), 255, 255);
    return dominant;
}

cv::Mat detect_dominant(cv::Mat image, cv::Vec3b colour, int s_threshold = 0, int v_threshold = 0) {
    // make a clone
    cv::Mat mono = image.clone();

    // get dominant hue
    int tgt_hue = reduce_colour(colour[0]);

    // pixels
    cv::Vec3b white(255, 255, 255);
    cv::Vec3b black(0, 0, 0);

    // scan image, set right pixels to white, rest to black
    for(int row = 0; row < mono.rows; ++row) {
		cv::Vec3b* current = mono.ptr<cv::Vec3b>(row);

        for(int col = 0; col < mono.cols; ++col) {
            // reduce the colour
            int hue = reduce_colour(current[col][0]);

            if(hue == tgt_hue && current[col][1] >= s_threshold && current[col][2] >= v_threshold) {
                current[col] = white;
            }
            else {
                current[col] = black;
            }
        }
	}

    return mono;
}
