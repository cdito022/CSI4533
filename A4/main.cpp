#include <cstdio>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "cmdline.hpp"
#include "homography.hpp"
#include "transformation.hpp"
#include "util.hpp"

#include "SequenceReader.hpp"
#include "DirectoryReader.hpp"
#include "VideoReader.hpp"

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
	auto parser = setup_parser();
	parser.parse_check(argc, argv);

	// get that one required argument
	std::string path;
	if(parser.rest().size() > 0) {
		path = parser.rest()[0];
	}
	else {
		std::cerr << "Must specify <path> of video file or image folder to read from." << std::endl << parser.usage();
		exit(EXIT_FAILURE);
	}

	// open reader
	SequenceReader* reader = nullptr;
	if(parser.exist("directory")) {
		reader = new DirectoryReader(path);
	}
	else {
		reader = new VideoReader(path);
	}

	// check that yes, we got this
	if(!reader->isOpened()) {
		std::cerr << "Error opening sequence." << std::endl;
		exit(EXIT_FAILURE);
	}

	// grab the frames
	std::vector<cv::Mat> frames;
	while(reader->hasFrames()) {
		frames.push_back(reader->getNextFrame());
	}

	// adios reader
	delete reader;

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

	// MASTER IMAGE
	cv::Mat& master = frames[0];

	// current stretches
	int top = 0, bottom = master.rows, left = 0, right = master.cols;

	for(int i = 1; i < frames.size(); ++i) {
		auto& frame = frames[i];
		auto& homography = homographies[i];

		if(homography.with == 0) {
			// STITCHING STARTS HERE

			// transform corners
			auto corners = transform_corners(frame, homography.matrix.inv());

			// find if anything goes above the current stretch
			int minY = top, maxY = bottom, minX = left, maxX = right;
			for(auto c : corners) {
				int x = (int)c.x, y = (int)c.y;
				minX = std::min(x, minX);
				maxX = std::max(x, maxX);
				minY = std::min(y, minY);
				maxY = std::max(y, maxY);
			}

			// stretch master in required directions
			cv::copyMakeBorder(master, master, top-minY, maxY-bottom, left-minX, maxX-right, cv::BORDER_CONSTANT, cv::Scalar(0));

			// register stretches
			top = minY; bottom = maxY; left = minX; right = maxX;

			// add stretches to homography
			// stretches left and top are always negative or 0
			homography.matrix.at<double>(0,2)+= left;
			homography.matrix.at<double>(1,2)+= top;

			// warp frame
			cv::warpPerspective(frame, frame, homography.matrix.inv(), cv::Size(master.cols, master.rows));

			// add frame to master
			cv::Vec3b black(0,0,0);
			for(int row = 0; row < master.rows; ++row) {
				for(int col = 0; col < master.cols; ++col) {
					if(master.at<cv::Vec3b>(row,col) == black && frame.at<cv::Vec3b>(row,col) != black) {
						master.at<cv::Vec3b>(row,col) = frame.at<cv::Vec3b>(row,col);
					}
				}
			}
		}
	}

	// do we save to file?
	if(parser.exist("output")) {
		cv::imwrite(parser.get<std::string>("output"), master);
	}

	display_image("ANOTHER BRICK IN THE WALL", master);
	cv::waitKey(0);

	return 0;
}
