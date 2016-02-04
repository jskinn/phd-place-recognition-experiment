/*
* AverageDifferenceMaskGenerator.cpp
*
*  Created on: 28 Jan 2016
*      Author: john
*/

#include "stdafx.h"
#include "DatasetImage.h"
#include "AverageDifferenceMaskGenerator.h"
#include "ThresholdSalienceMask.h"

AverageDifferenceMaskGenerator::AverageDifferenceMaskGenerator()
{
}

AverageDifferenceMaskGenerator::~AverageDifferenceMaskGenerator()
{
}

/**
* Generate a salience mask, by comparing
* TODO: This needs tweaking, it's assuming the indexes in the datasets match ground truth,
* It's also generating a bad salience mask.
*/
ImageMatcherInterface* AverageDifferenceMaskGenerator::generateSalienceMask(
	const ImageDatasetInterface& reference,
	const ImageDatasetInterface& query) const
{
	int datasetSize = std::min(reference.count(), query.count());
	cv::Mat avgDiff;

	for (int index = 0; index < datasetSize; ++index) {
		// Read images and validate input. Move to next pair if there's as problem.
		DatasetImage referenceImage = reference.get(index);
		DatasetImage queryImage = query.get(index);
		if (&referenceImage == NULL || &queryImage == NULL) {
			continue;
		}

		// Calculate the difference image
		cv::Mat diffImage;
		cv::absdiff(referenceImage.getImage(), queryImage.getImage(), diffImage);

		if (index == 0) {
			diffImage.convertTo(avgDiff, CV_32FC1, 1 / 255.0);
		}
		else {
			diffImage.convertTo(diffImage, CV_32FC1, 1 / 255.0);
			avgDiff += diffImage;
		}
	}
	//TODO: This needs to be inverted somehow?
	return new ThresholdSalienceMask(cv::Mat(avgDiff / datasetSize), 0.5);
}
