#ifndef DIRECTORYREADER_HPP
#define DIRECTORYREADER_HPP

#include "SequenceReader.hpp"

#include <dirent.h>
#include <string>
#include <vector>

#include <opencv2/core.hpp>

class DirectoryReader : public SequenceReader {
private:
	std::string dirname;
	std::vector<std::string> filenames;
	int cursor;

public:
	DirectoryReader(std::string dir);

	// returns the next image in the directory
	cv::Mat getNextFrame();

	// returns whether or not there are images left in the directory
	bool hasFrames();

	// returns whether or not the directory was opened properly
	bool isOpened();
};

#endif
