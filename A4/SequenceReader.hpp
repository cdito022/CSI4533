#ifndef SEQUENCEREADER_HPP
#define SEQUENCEREADER_HPP

#include <opencv2/core.hpp>

class SequenceReader {
public:
	// returns the next frame in the sequence
	virtual cv::Mat getNextFrame() = 0;

	// returns whether or not there are frames left
	virtual bool hasFrames() = 0;

	// returns whether or not the sequence is properly opened
	virtual bool isOpened() = 0;
};

#endif
