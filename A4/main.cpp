#include <cstdio>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#include "homography.hpp"
#include "SequenceReader.hpp"
#include "transformation.hpp"
#include "util.hpp"

#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

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

	// put all the frames in relation to frame 0
	chain_homographies(homographies);

	// homographies[n].matrix contains the transformation
	// that should be applied to frame 0 so it aligns

	/*
		TODO:
		- use frame 0 as master image
		- compute transformed corners for each image n where n > 0 in the sequence
		- find min and max x and y after transformation
		- stretch master image to accomodate found dimensions
		- adjust each homography according to found dimensions min x and y
		- warpPerspective for each image n where n > 0
		- bring pixels over to master image
	 */

	// first image and image that will be added
	cv::Mat master = frames[0];
	int n = 1;
	cv::Mat addon = frames[n];
	auto& homography = homographies[n];


	// find new corners after perspective transform
	auto corners = transform_corners(addon, homography.matrix.inv());

	int minX = 0, maxX = 0, minY = 0, maxY = 0;

	for(auto p : corners) {
		int x = (int)p.x;
		int y = (int)p.y;

		printf("(%d,%d)\n", x, y);

		minX = std::min(x, minX);
		maxX = std::max(x, maxX);
		minY = std::min(y, minY);
		maxY = std::max(y, maxY);
	}

	printf("minX = %d, maxX = %d, minY = %d, maxY = %d\n", minX, maxX, minY, maxY);
	// END find new corners

	// enlarge image to make room
	int top = std::max(0-minY, 0);
	int bottom = std::max(0, maxY-addon.rows);
	int left = std::max(0-minX, 0);
	int right = std::max(0, maxX-addon.cols);
	printf("top = %d, bottom = %d, left = %d, right = %d\n", top, bottom, left, right);
	cv::copyMakeBorder(addon, addon, top, bottom, left, right, cv::BORDER_CONSTANT, cv::Scalar(0));

	// shift homography
	if(minX < 0) {
		homography.matrix.at<float>(0,2)-= minX;
	}
	if(minY < 0) {
		homography.matrix.at<float>(1,2)-= minY;
	}

	// warp this shit
	cv::warpPerspective(addon, addon, homography.matrix, cv::Size(addon.cols, addon.rows), cv::WARP_INVERSE_MAP);

	display_image("Master", master);
	display_image("Add-on", addon);

	cv::waitKey(0);

	return 0;
}
