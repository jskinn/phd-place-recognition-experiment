/*
* PairwiseSalienceMaskGenerator.cpp
*
*  Created on: 29 Jan 2016
*      Author: john
*/

#include "stdafx.h"
#include "PairwiseSalienceMaskGenerator.h"

PairwiseSalienceMaskGenerator::PairwiseSalienceMaskGenerator(int indexProximityThreshold) :
	indexProx(indexProximityThreshold)
{
	if (indexProx < 0) {
		indexProx = 0;
	}
}

PairwiseSalienceMaskGenerator::~PairwiseSalienceMaskGenerator()
{
}

/**
* Overload generate salience mask, without any image filters.
*/
void PairwiseSalienceMaskGenerator::generateSalienceMask(
	const ImageDatasetInterface& reference,
	const ImageDatasetInterface& query,
	cv::Mat& outputMask) const
{
	std::list<ImageFilterInterface*> filters;
	return this->generateSalienceMask(reference, query, filters, outputMask);
}

/**
* Generate a salience mask, by comparing all query images to all reference images.
* This one actually has a modcum of thought put into the mask it outputs.
*/
void PairwiseSalienceMaskGenerator::generateSalienceMask(
	const ImageDatasetInterface& reference,
	const ImageDatasetInterface& query,
	std::list<ImageFilterInterface*>& filters,
	cv::Mat& outputMask) const
{
	cv::Mat avgSame, avgDifferent;
	int sameCount = 0, differentCount = 0;
	for (int i = 0; i < query.count(); ++i) {
		// Load the query image and apply filters
		cv::Mat queryImage = query.get(i);
		if (queryImage.empty()) {
			continue;
		}

		for (int j = 0; j < reference.count(); ++j) {
			// Load the reference image and apply filters
			cv::Mat referenceImage = reference.get(j);
			if (referenceImage.empty()) {
				continue;
			}

			// Take the absolute difference between the reference and query images.
			cv::Mat diffImage;
			cv::absdiff(referenceImage, queryImage, diffImage);

			// If the images indexes are close together, consider them as the "same" place
			// Maintain separate average differences for matching and non-matching 
			if (std::abs(i - j) <= this->indexProx) {
				if (sameCount == 0) {
					diffImage.convertTo(avgSame, CV_32FC1, 1 / 255.0);
				} else {
					diffImage.convertTo(diffImage, CV_32FC1, 1 / 255.0);
					avgSame += diffImage;
				}
				sameCount++;
			} else {
				if (differentCount == 0) {
					diffImage.convertTo(avgDifferent, CV_32FC1, 1 / 255.0);
				}
				else {
					diffImage.convertTo(diffImage, CV_32FC1, 1 / 255.0);
					avgDifferent += diffImage;
				}
				differentCount++;
			}
		}
	}

	// Take the average matching difference and the average different difference
	avgSame /= (float)sameCount;
	avgDifferent /= (float)differentCount;

	// Use the differences between the matching and non-matching as the saliency mask
	cv::absdiff(avgSame, avgDifferent, outputMask);
}
