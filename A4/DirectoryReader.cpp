#include "DirectoryReader.hpp"

#include <algorithm>
#include <cstring>
#include <dirent.h>
#include <iostream>
#include <string>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#define FRAME_GAP 30

DirectoryReader::DirectoryReader(std::string dir): dirname(dir), cursor(0) {
	if(dir.back() != '/') {
		dir.push_back('/');
	}

	auto directory = opendir(dir.c_str());
	dirent* e = nullptr;

	while( (e = readdir(directory)) != nullptr ) {
		// skip hidden files and same/parent directory links
		if(e->d_name[0] != '.') {
			filenames.push_back(e->d_name);
		}
	}

	closedir(directory);
	std::sort(filenames.begin(), filenames.end());
}

cv::Mat DirectoryReader::getNextFrame() {
	if(hasFrames()) {
		std::string& filename = filenames[cursor++];
		return cv::imread(dirname + filename);
	}
	else {
		return cv::Mat();
	}
}

bool DirectoryReader::hasFrames() {
	return cursor < filenames.size();
}

bool DirectoryReader::isOpened() {
	return filenames.size() > 0;
}
