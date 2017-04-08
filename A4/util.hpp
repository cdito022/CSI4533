#ifndef UTIL_HPP
#define UTIL_HPP

#include <memory>

#include "SequenceReader.hpp"

#include <opencv2/core/core.hpp>

std::shared_ptr<SequenceReader> reader_from_args(int argc, char* argv[]);

void display_image(std::string name, cv::Mat image);

#endif
