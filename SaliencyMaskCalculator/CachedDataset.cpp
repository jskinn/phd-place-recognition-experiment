/*
* CachedDataset.cpp
*
*  Created on: 27 Jan 2016
*      Author: john
*/

#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "CachedDataset.h"


CachedDataset::CachedDataset(std::string prefix, std::string suffix, int count, int initialIndex, int step, int pad, const std::list<ImageFilterInterface*>& filters) : _count(count)
{
	int index;

	// Allocate storage for all the images
	this->imageStorage = new cv::Mat[count];

	// Load all the images
	for (index = 0; index < count; index++) {
		// Build the filename for the image
		std::stringstream filenameBuilder;
		int id = initialIndex + (step * index);
		filenameBuilder << prefix;
		int tempPad = pad;
		while (id < tempPad) {
			filenameBuilder << '0';
			tempPad /= 10;
		}
		filenameBuilder << id << suffix;

		// Load the image
		this->imageStorage[index] = cv::imread(filenameBuilder.str().c_str(), cv::IMREAD_COLOR);

		// Apply the filters
		if (this->imageStorage[index].empty()) {
			std::cout << "Could not load image " << filenameBuilder.str() << std::endl;
		} else if (filters.size() > 0) {
			ImageFilterInterface::applyFilters(this->imageStorage[index], filters);
		}
	}
}

CachedDataset::~CachedDataset()
{
	delete[] this->imageStorage;
}

int CachedDataset::count() const {
	return this->_count;
}

cv::Mat& CachedDataset::get(int index) const {
	if (index >= 0 && index < this->_count) {
		return this->imageStorage[index];
	}
	return this->imageStorage[0];
}