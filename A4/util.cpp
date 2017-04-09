#include "util.hpp"

#include <string>

#include "cmdline.hpp"

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

void display_image(std::string name, cv::Mat image) {
    cv::namedWindow(name);
    cv::imshow(name, image);
}

cmdline::parser setup_parser() {
	cmdline::parser parser;

	parser.add<std::string>("output", 'o', "Output file to write stitched image to.", false);
	parser.add("directory", 'd', "Indicate that the passed argument is a directory from which images should be loaded.");
	// add MAGIC argument, etc.

	parser.footer("<path>");

	return parser;
}
