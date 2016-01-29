/*
* AverageDifferenceMaskGenerator.cpp
*
*  Created on: 28 Jan 2016
*      Author: john
*/

#include "stdafx.h"
#include "AverageDifferenceMaskGenerator.h"

AverageDifferenceMaskGenerator::AverageDifferenceMaskGenerator()
{
}

AverageDifferenceMaskGenerator::~AverageDifferenceMaskGenerator()
{
}

/**
* Overload generate salience mask, without any image filters.
*/
void AverageDifferenceMaskGenerator::generateSalienceMask(
	const ImageDatasetInterface& reference,
	const ImageDatasetInterface& query,
	cv::Mat& outputMask) const
{
	std::list<ImageFilterInterface*> filters;
	return this->generateSalienceMask(reference, query, filters, outputMask);
}

/**
* Generate a salience mask, by comparing
* TODO: This needs tweaking, it's assuming the indexes in the datasets match ground truth,
* It's also generating a bad salience mask.
*/
void AverageDifferenceMaskGenerator::generateSalienceMask(
	const ImageDatasetInterface& reference,
	const ImageDatasetInterface& query,
	std::list<ImageFilterInterface*>& filters,
	cv::Mat& outputMask) const
{
	int datasetSize = std::min(reference.count(), query.count());
	cv::Mat avgDiff;

	for (int index = 0; index < datasetSize; ++index) {
		// Read images and validate input. Move to next pair if there's as problem.
		cv::Mat referenceImage = reference.get(index);
		cv::Mat queryImage = query.get(index);
		if (referenceImage.empty() || queryImage.empty()) {
			continue;
		}

		// Apply all the filters to the images
		ImageFilterInterface::applyFilters(referenceImage, filters);
		ImageFilterInterface::applyFilters(queryImage, filters);
		cv::Mat diffImage;
		cv::absdiff(referenceImage, queryImage, diffImage);

		if (index == 0) {
			diffImage.convertTo(avgDiff, CV_32FC1, 1 / 255.0);
		}
		else {
			diffImage.convertTo(diffImage, CV_32FC1, 1 / 255.0);
			avgDiff += diffImage;
		}
	}
	//TODO: This needs to be inverted somehow?
	outputMask = avgDiff / datasetSize;
}
