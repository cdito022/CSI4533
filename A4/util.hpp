#ifndef UTIL_HPP
#define UTIL_HPP

#include "cmdline.hpp"

#include <opencv2/core/core.hpp>

void display_image(std::string name, cv::Mat image);

cmdline::parser setup_parser();

#endif
