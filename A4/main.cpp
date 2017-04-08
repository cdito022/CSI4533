#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#include "homography.hpp"
#include "SequenceReader.hpp"
#include "util.hpp"

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#define NUM_THREADS 4

void homography_thread(	std::vector<cv::Mat>* frames,
						std::vector<homography_descriptor>* homographies,
						int* pos, std::mutex* mtx) {
	while(true) {
		// get next frame id
		mtx->lock();
		int i = (*pos)++;
		mtx->unlock();

		// make sure we are on a frame
		if(i >= frames->size()) {
			return;
		}
		// pick frame
		cv::Mat frame = frames->at(i);

		// find match
		homographies->at(i) = find_best_homography(*frames, i);
	}
}

int main(int argc, char* argv[]) {
	// open and validate
	auto reader = reader_from_args(argc, argv);
	if(!reader->isOpened()) {
		std::cerr << "Error opening sequence." << std::endl;
		exit(EXIT_FAILURE);
	}

	// grab the frames
	std::vector<cv::Mat> frames;
	while(reader->hasFrames()) {
		frames.push_back(reader->getNextFrame());
	}

	// compute homographies
	std::vector<struct homography_descriptor> homographies(frames.size());
	std::vector<std::thread> threads;
	int pos = 0;
	std::mutex mtx;

	// spin threads
	for(int i = 0; i < NUM_THREADS; ++i) {
		threads.push_back(std::thread(homography_thread, &frames, &homographies, &pos, &mtx));
	}
	// catch threads
	for(int i = 0; i < threads.size(); ++i) {
		threads[i].join();
	}

	std::cout << "Initial homographies:" << std::endl;
	for(int i = 0; i < homographies.size(); ++i) {
		// some debug information
		struct homography_descriptor& homography = homographies[i];
		if(homography.with == -1) {
			std::cout << "No match found for image " << i << std::endl;
		}
		else {
			std::cout << "Frame " << i << " has a match with frame " << homography.with << std::endl;
		}
	}

	chain_homographies(homographies);

	std::cout << std::endl << "Chained homographies:" << std::endl;
	for(int i = 0; i < homographies.size(); ++i) {
		// some debug information
		struct homography_descriptor& homography = homographies[i];
		if(homography.with == -1) {
			std::cout << "No match found for image " << i << std::endl;
		}
		else {
			std::cout << "Frame " << i << " has a match with frame " << homography.with << std::endl;
		}
	}

	return 0;
}
