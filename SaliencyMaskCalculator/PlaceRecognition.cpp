/*
* PlaceRecognition.cpp
*
*  Created on: 26 Jan 2016
*      Author: john
*/

#include "stdafx.h"
#include "DatasetImage.h"
#include "PlaceRecognition.h"

PlaceRecognition::PlaceRecognition()
{
}


PlaceRecognition::~PlaceRecognition()
{
}

/**
* Generate a diagonal matrix comparing each query image to all the reference images,
* and measuring the similar matrix.
* We want the matrix to have a bright diagonal and low values everywhere else.
*/
float PlaceRecognition::generateDiagonalMatrix(
	const ImageDatasetInterface& reference,
	const ImageDatasetInterface& query,
	const ImageMatcherInterface& imageMatcher,
	const SimilarityCriteria& similarityCriteria,
	cv::Mat& output) const
{
	output = cv::Mat::zeros(reference.count(), query.count(), CV_32FC1);

	// bail out early if there are no query images or no reference images, it saves us from a divide by zero later.
	if (query.count() <= 0 || reference.count() <= 0) {
		return 1.0f;
	}

	float similarMatchCount = 0.0f;
	for (int i = 0; i < query.count(); ++i) {
		float bestScore = std::numeric_limits<float>::infinity();
		bool bestIsSimilar = false;

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

			// Compute the similarity score based on the sum of absolute differences between the images.
			float matchScore = imageMatcher.matchImages(referenceImage.getImage(), queryImage.getImage());
			output.at<float>(j, i) = matchScore;

			// Track the lowest scoring (most similar) image, and whether it counts as 'close' to the query image.
			if (matchScore <= bestScore) {
				bestScore = matchScore;
				bestIsSimilar = similarityCriteria.isImageSimilar(queryImage, referenceImage);
			}
		}

		if (bestIsSimilar) {
			similarMatchCount++;
		}
	}

	assert(similarMatchCount <= query.count());
	return similarMatchCount / query.count();
}

/**
 * Calculate the percentage of best matching reference images that fall within a particualar distance
 * of the query image. Use the diagonal matrix to avoid recalculating the image difference,
 * but it sill requires the datasets because we need the ground truth for each image.
 */
float PlaceRecognition::recalculatePerformance(const ImageDatasetInterface& reference, const ImageDatasetInterface& query, const cv::Mat& diagonalMatrix, const SimilarityCriteria& similarityCriteria) const
{
	float similarMatchCount = 0.0f;

	// Exit early if there are no query images or no reference images, to save from a divide by 0.
	if (query.count() <= 0 || reference.count() <= 0) {
		return 1.0f;
	}

	for (int i = 0; i < query.count(); ++i) {
		// Load the query image
		DatasetImage queryImage = query.get(i);
		if (&queryImage == NULL) {
			continue;
		}

		float bestScore = 1.0f;	// Theoretical max value for the diagonal matrix at 
		bool bestIsSimilar = false;

		for (int j = 0; j < reference.count(); ++j) {
			// Load the reference image
			DatasetImage referenceImage = reference.get(j);
			if (&referenceImage == NULL) {
				continue;
			}

			float score = diagonalMatrix.at<float>(j, i);
			if (score < bestScore) {
				bestScore = score;
				bestIsSimilar = similarityCriteria.isImageSimilar(queryImage, referenceImage);
			}
		}

		if (bestIsSimilar) {
			similarMatchCount++;
		}
	}

	return similarMatchCount / query.count();
}
