#include "VideoReader.hpp"

#include <string>

#include <opencv2/core.hpp>

#define FRAME_GAP 30

VideoReader::VideoReader(std::string filename): capture(filename) {
}

VideoReader::~VideoReader() {
	capture.release();
}

cv::Mat VideoReader::getNextFrame() {
	cv::Mat frame;
	
	// subtly load next frame
	if(hasFrames()) {
		// set frame
		frame = nextFrame;
		nextFrame.release();
	}
	else {
		// return empty image
		return cv::Mat();
	}

	// skip other frames
	for(int i = FRAME_GAP - 1; i > 0; --i) {
		capture.grab();
	}

	return frame;
}

bool VideoReader::hasFrames() {
	// will read on the first call
	// simply return true on subsequent calls
	return nextFrame.data != nullptr || capture.read(nextFrame);
}

bool VideoReader::isOpened() {
	return capture.isOpened();
}
