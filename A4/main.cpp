#include <cstdio>
#include <iostream>
#include <mutex>
#include <string>
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

	// HERE WE GO

	// MASTER IMAGE
	cv::Mat& master = frames[0];

	// compute transformed corners for all other images
	int	minX = 0,
		maxX = master.cols,
		minY = 0,
		maxY = master.rows;

	for(int i = 1; i < frames.size(); ++i) {
		auto& frame = frames[i];
		auto& homography = homographies[i];

		if(homography.with == 0) { // skip bad frames
			auto corners = transform_corners(frame, homography.matrix.inv());
			// find min and max x and y after transformation
			for(auto p : corners) {
				int x = (int)p.x, y = (int)p.y;
				minX = std::min(x, minX);
				maxX = std::max(x, maxX);
				minY = std::min(y, minY);
				maxY = std::max(y, maxY);
			}
		}
	}

	// stretch master image to accomodate found dimensions
	cv::copyMakeBorder(master, master, -minY, maxY-master.rows, -minX, maxX-master.cols, cv::BORDER_CONSTANT, cv::Scalar(0));

	// adjust each homography according to found dimensions min x and y
	// warp all images to a frame the same size as master
	for(int i = 1; i < frames.size(); ++i) {
		auto& frame = frames[i];
		auto& homography = homographies[i];

		if(homography.with == 0) { // skip bad frames
			homography.matrix.at<double>(0,2)+= std::min(0, minX);
			homography.matrix.at<double>(1,2)+= std::min(0, minY);

			cv::warpPerspective(frame, frame, homography.matrix.inv(), cv::Size(master.cols, master.rows));
		}
	}

	std::cout << "time to transpose" << std::endl;

	// transpose pixels to master image
	cv::Vec3b black(0,0,0); // comes in handy

	for(int i = 1; i < frames.size(); ++i) {
		auto& frame = frames[i];
		auto& homography = homographies[i];

		if(homography.with == 0) { // skip bad frames
			for(int row = 0; row < frame.rows; ++row) {
				for(int col = 0; col < frame.cols; ++col) {
					if(master.at<cv::Vec3b>(row, col) == black && frame.at<cv::Vec3b>(row, col) != black) {
						master.at<cv::Vec3b>(row, col) = frame.at<cv::Vec3b>(row, col);
					}
				}
			}
		}
	}

	// DEBUG: display frames;
	/*for(int i = 0; i < frames.size(); ++i) {
		if(i == 0 || homographies[i].with == 0) {
			display_image("Viewer", frames[i]);
			cv::waitKey(0);
		}
	}*/

	display_image("ANOTHER BRICK IN THE WALL", master);
	cv::waitKey(0);

	return 0;











	// DEBUG STUFF

	// first image and image that will be added
	/*cv::Mat master = frames[0];
	int n = 1;
	cv::Mat addon = frames[n];
	auto& homography = homographies[n];


	// find new corners after perspective transform
	auto corners = transform_corners(addon, homography.matrix.inv());

	int minX = 0, maxX = master.cols, minY = 0, maxY = master.rows;

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
	cv::copyMakeBorder(master, master, 0-minY, maxY-master.rows, 0-minX, maxX-master.cols, cv::BORDER_CONSTANT, cv::Scalar(0));
	cv::copyMakeBorder(addon, addon, 0-minY, maxY-addon.rows, 0-minX, maxX-addon.cols, cv::BORDER_CONSTANT, cv::Scalar(0));

	display_homography_matrix(homography.matrix);

	// shift homography
	//homography.matrix.at<float>(0,2)-= std::min(0,minX);
	//homography.matrix.at<float>(1,2)-= std::min(0,minY);

	display_homography_matrix(homography.matrix);

	// warp this shit
	cv::warpPerspective(addon, addon, homography.matrix, cv::Size(addon.cols, addon.rows), cv::WARP_INVERSE_MAP);

	cv::Vec3b black(0,0,0);
	for(int row = 0; row < master.rows; ++row) {
		for(int col = 0; col < master.cols; ++col) {
			if(master.at<cv::Vec3b>(row,col) == black && addon.at<cv::Vec3b>(row,col) != black) {
				master.at<cv::Vec3b>(row,col) = addon.at<cv::Vec3b>(row,col);
			}
		}
	}

	//display_image("Master", master);
	//display_image("Add-on", addon);

	cv::waitKey(0);*/

	return 0;
}
