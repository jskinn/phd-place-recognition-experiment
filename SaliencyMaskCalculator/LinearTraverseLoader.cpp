/*
* LinearTraverseLoader.cpp
*
*  Created on: 2 Feb 2016
*      Author: john
*/

#include "stdafx.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "LinearTraverseLoader.h"


LinearTraverseLoader::LinearTraverseLoader(
	cv::Vec3d startLocation,
	cv::Vec3d finalLocation,
	cv::Vec3d orientation,
	std::string filenamePrefix,
	std::string filenameSuffix,
	int imageCount,
	int initialIndex,
	int indexStep,
	int indexPad,
	const std::list<ImageFilterInterface*>& filters) :

	startLocation(startLocation),
	orientation(orientation),
	filenamePrefix(filenamePrefix),
	filenameSuffix(filenameSuffix),
	imageCount(imageCount),
	initialIndex(initialIndex),
	indexStep(indexStep),
	indexPad(indexPad),
	filters(filters)
{
	locationStep = (finalLocation - startLocation) / imageCount;
}


LinearTraverseLoader::~LinearTraverseLoader()
{
}

int LinearTraverseLoader::getCount()
{
	return this->imageCount;
}

void LinearTraverseLoader::loadImages(std::vector<DatasetImage*>& destination, int indexOffset)
{
	for (int index = 0; index < imageCount; index++) {
		std::stringstream filenameBuilder;
		cv::Mat image;

		// Build the filename for the image
		int id = initialIndex + (indexStep * index);
		filenameBuilder << filenamePrefix;
		int tempPad = indexPad;
		while (id < tempPad) {
			filenameBuilder << '0';
			tempPad /= 10;
		}
		filenameBuilder << id << filenameSuffix;

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
		destination[index + indexOffset] = new DatasetImage(image, this->startLocation + index * this->locationStep, this->orientation);
	}
}
