#include "util.hpp"

#include <cstring>
#include <iostream>
#include <memory>

#include "SequenceReader.hpp"
#include "DirectoryReader.hpp"
#include "VideoReader.hpp"

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

void usage(char* exec) {
	std::cout << "USAGE:\t" << exec << " [-d] <target>" << std::endl
			  << "\t-d: Use <target> as a directory containing stills of the sequence." << std::endl;
}

std::shared_ptr<SequenceReader> reader_from_args(int argc, char* argv[]) {
	if(argc == 2) {
		return std::shared_ptr<SequenceReader>(new VideoReader(argv[1]));
	}
	else if(argc == 3) {
		if(std::strcmp(argv[1], "-d") == 0) {
			// make, check and return dir reader from argv[2]
			return std::shared_ptr<SequenceReader>(new DirectoryReader(argv[2]));
		}
		else {
			usage(argv[0]);
			return nullptr;
		}
	}
	usage(argv[0]);
	return nullptr;
}

void display_image(std::string name, cv::Mat image) {
    cv::namedWindow(name);
    cv::imshow(name, image);
}
