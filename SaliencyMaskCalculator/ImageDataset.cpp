#include "stdafx.h"
#include "ImageDataset.h"

#include <iostream>
#include <sstream>
#include <opencv2/highgui/highgui.hpp>

ImageDataset::ImageDataset(std::string prefix, std::string suffix, int count, int initialIndex, int step, int pad) :
	prefix(prefix),
	suffix(suffix),
	size(count),
	start(initialIndex),
	step(step),
	pad(pad)
{
}


ImageDataset::~ImageDataset()
{
}

int ImageDataset::count() const {
	return this->size;
}

bool ImageDataset::get(int index, cv::Mat& output) const {
	std::string filename = this->getFilename(index);
	output = cv::imread(filename, cv::IMREAD_COLOR);
	if (output.empty())                                      // Check for invalid input
	{
		std::cout << "Could not open or find the reference image: " << filename << std::endl;
		return false;
	}
	return true;
}

std::string ImageDataset::getFilename(int index) const {
	index = this->start + this->step * index;
	std::stringstream s;
	s << this->prefix;
	int pad = this->pad;
	while (index < pad) {
		s << '0';
		pad /= 10;
	}
	s << index << this->suffix;
	return s.str();
}

