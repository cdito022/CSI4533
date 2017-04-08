#include <iostream>

#include "SequenceReader.hpp"
#include "util.hpp"

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

int main(int argc, char* argv[]) {
	auto reader = reader_from_args(argc, argv);

	if(!reader->isOpened()) {
		std::cerr << "Error opening sequence." << std::endl;
		exit(EXIT_FAILURE);
	}

	while(reader->hasFrames()) {
		auto frame = reader->getNextFrame();
		display_image("Current frame", frame);
		cv::waitKey(0);
	}

	return 0;
}
