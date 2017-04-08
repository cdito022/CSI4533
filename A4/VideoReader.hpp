#ifndef VIDEOREADER_HPP
#define VIDEOREADER_HPP

#include "SequenceReader.hpp"

#include <string>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

class VideoReader : public SequenceReader {
private:
	cv::VideoCapture capture;
	cv::Mat nextFrame;

public:
	VideoReader(std::string filename);
	~VideoReader();

	// returns the next frame in the video
	cv::Mat getNextFrame();

	// returns whether or not we're at the end of the video
	bool hasFrames();

	// returns whether or not the video file was properly opened
	bool isOpened();
};

#endif
