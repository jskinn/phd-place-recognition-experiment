/*
* LinearTraverseLoader.cpp
*
*  Created on: 2 Feb 2016
*      Author: john
*/

#include "stdafx.h"
#include "VPRiCELoader.h"
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>


VPRiCELoader::VPRiCELoader(std::string folderPath, int imageCount, int initialIndex, int step, const std::list<ImageFilterInterface*>& filters) :
	folderPath(folderPath),
	imageCount(imageCount),
	initialIndex(initialIndex),
	step(step),
	filters(filters)
{
}

VPRiCELoader::~VPRiCELoader()
{
}

int VPRiCELoader::getCount()
{
	return this->imageCount;
}

void VPRiCELoader::loadImages(std::vector<DatasetImage*>& destination, int indexOffset)
{
	for (int index = 0; index < imageCount; index++) {
		std::stringstream filenameBuilder;
		cv::Mat image;

		// Build the filename for the image
		int imageIndex = initialIndex + step * index;
		filenameBuilder << folderPath << "image-";
		int tempPad = 10000;
		while (tempPad > imageIndex && tempPad > 1) {
			filenameBuilder << '0';
			tempPad /= 10;
		}
		filenameBuilder << imageIndex << ".png";

		// Load the image, if we can.
		image = cv::imread(filenameBuilder.str().c_str(), cv::IMREAD_COLOR);
		if (image.empty()) {
			std::cout << "Could not load image " << filenameBuilder.str() << std::endl;
			destination[index + indexOffset] = NULL;
			continue;
		}

		// Apply the filters
		if (filters.size() > 0) {
			ImageFilterInterface::applyFilters(image, filters);
		}

		// Create the DatasetImage object. It is the responsibility of the dataset to delete it later.
		destination[index + indexOffset] = new DatasetImage(image, cv::Vec3d(index, 0.0, 0.0), cv::Vec3d(0.0,0.0,0.0));
	}
}
