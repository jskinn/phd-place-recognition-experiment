/*
* PairwiseSalienceMaskGenerator.cpp
*
*  Created on: 29 Jan 2016
*      Author: john
*/

#include "stdafx.h"
#include "PairwiseSalienceMaskGenerator.h"

PairwiseSalienceMaskGenerator::PairwiseSalienceMaskGenerator(const SimilarityCriteria& similarityCriteria) :
	criteria(similarityCriteria)
{
}

PairwiseSalienceMaskGenerator::~PairwiseSalienceMaskGenerator()
{
}

/**
* Generate a salience mask, by comparing all query images to all reference images.
* This one actually has a modcum of thought put into the mask it outputs.
*/
void PairwiseSalienceMaskGenerator::generateSalienceMask(
	const ImageDatasetInterface& reference,
	const ImageDatasetInterface& query,
	cv::Mat& outputMask) const
{
	cv::Mat avgSame, avgDifferent;
	int sameCount = 0, differentCount = 0;
	for (int i = 0; i < query.count(); ++i) {
		// Load the query image
		DatasetImage queryImage = query.get(i);
		if (&queryImage == NULL) {
			continue;
		}

		for (int j = 0; j < reference.count(); ++j) {
			// Load the reference image
			DatasetImage referenceImage = reference.get(j);
			if (&referenceImage == NULL) {
				continue;
			}

			// Take the absolute difference between the reference and query images.
			cv::Mat diffImage;
			cv::absdiff(referenceImage.getImage(), queryImage.getImage(), diffImage);

			// If the images indexes are close together, consider them as the "same" place
			// Maintain separate average differences for matching and non-matching 
			if (criteria.isImageSimilar(queryImage, referenceImage)) {
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
