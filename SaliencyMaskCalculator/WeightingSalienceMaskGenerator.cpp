/*
* WeightingSalienceMaskGenerator.cpp
*
*  Created on: 4 Feb 2016
*      Author: john
*/

#include "stdafx.h"
#include "WeightingSalienceMaskGenerator.h"
#include "LogicalSalienceMask.h"
#include "SimpleComparisonMask.h"

#include <opencv2/highgui/highgui.hpp>	// Debug

WeightingSalienceMaskGenerator::WeightingSalienceMaskGenerator(const SimilarityCriteria& similarityCriteria, float salienceFraction, std::string outputDebugDir) :
	criteria(similarityCriteria), salienceFraction(salienceFraction), outputDebug(outputDebugDir)
{
	assert(salienceFraction >= 0.0f);
	assert(salienceFraction <= 1.0f);
}


WeightingSalienceMaskGenerator::~WeightingSalienceMaskGenerator()
{
}

/**
* Generate a salience mask, by comparing all query images to all reference images.
* This one actually has a modcum of thought put into the mask it outputs.
*/
ImageMatcherInterface* WeightingSalienceMaskGenerator::generateSalienceMask(
	const ImageDatasetInterface& reference,
	const ImageDatasetInterface& query) const
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
				}
				else {
					diffImage.convertTo(diffImage, CV_32FC1, 1 / 255.0);
					avgSame += diffImage;
				}
				sameCount++;
			}
			else {
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
	assert(sameCount > 0);
	assert(differentCount > 0);
	avgSame /= (float)sameCount;
	avgDifferent /= (float)differentCount;

	// Debug, show the average differences.
	cv::Mat outputImage;
	avgSame.convertTo(outputImage, CV_8UC1, 255.0);
	cv::imwrite(outputDebug + "\\average difference matching.png", outputImage);

	avgDifferent.convertTo(outputImage, CV_8UC1, 255.0);
	cv::imwrite(outputDebug + "\\average difference non-matching.png", outputImage);

	// Use the differences between the matching and non-matching as the saliency mask
	return new SimpleComparisonMask(avgSame, avgDifferent, salienceFraction, outputDebug);
}