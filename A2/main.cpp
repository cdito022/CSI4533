#include <iostream>
#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

int main(int argc, char* argv[]) {
	// take video file in, or the webcam
	cv::VideoCapture capture;
	if(argc > 1) {
		capture = cv::VideoCapture(argv[1]);
	}
	else {
		capture = cv::VideoCapture(0);
	}

	// if opening failed
	if(!capture.isOpened()) {
		std::cerr << "Error reading video sequence." << std::endl;
		exit(EXIT_FAILURE);
	}

	double rate = capture.get(CV_CAP_PROP_FPS);
	int delay = 1000 / rate;

	cv::Mat frame;
	cv::namedWindow("Extracted frame");

	while(true) {
		if(!capture.read(frame)) {
			break;
		}

		// convert to grayscale for hough
		cv::Mat grayscale;
		cv::cvtColor(frame, grayscale, CV_BGR2GRAY);
		cv::GaussianBlur(grayscale, grayscale, cv::Size(5,5), 1.5);

		// detect circles
		std::vector<cv::Vec3f> circles;
		cv::HoughCircles(grayscale, circles, CV_HOUGH_GRADIENT, 2, 50, 200, 100, 25, 100);

		// draw circles
		for(auto it = circles.begin(); it != circles.end(); ++it) {
			cv::circle(frame, cv::Point((*it)[0], (*it)[1]), (*it)[2], cv::Scalar(255), 2);
		}

		cv::imshow("Extracted frame", frame);

		if(cv::waitKey(delay) >= 0) {
			break;
		}
	}

	capture.release();

	return 0;
}
