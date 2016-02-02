/*
* PlaceRecognition.cpp
*
*  Created on: 26 Jan 2016
*      Author: john
*/

#include "stdafx.h"
#include <algorithm>
#include <assert.h>
#include <opencv2/highgui/highgui.hpp>
#include "DatasetImage.h"
#include "NullSalienceMask.h"
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
	const SalienceMaskInterface& salienceMask,
	const SimilarityCriteria& similarityCriteria,
	cv::Mat& output) const
{
	output = cv::Mat::zeros(reference.count(), query.count(), CV_32FC1);

	float similarMatchCount = 0.0f;
	for (int i = 0; i < query.count(); ++i) {
		float bestScore = 1.0f;
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

			// Take the absolute difference between the reference and query images.
			cv::Mat diffImage;
			cv::absdiff(referenceImage.getImage(), queryImage.getImage(), diffImage);
			diffImage.convertTo(diffImage, CV_32FC1, 1 / 255.0);

			// Mask the image throug the salience mask
			salienceMask.applyMask(diffImage);

			// Compute the similarity score based on the sum of absolute differences between the images.
			cv::Scalar sum = cv::sum(diffImage);
			int numPixels = (diffImage.rows * diffImage.cols) - salienceMask.getNumberOfRemovedPixels();
			float matchScore = (float)sum[0] / numPixels;
			assert(matchScore >= 0.0f);
			assert(matchScore <= 1.0f);
			output.at<float>(i,j) = matchScore;

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

	return similarMatchCount / query.count();
}

float PlaceRecognition::generateDiagonalMatrix(
	const ImageDatasetInterface& reference,
	const ImageDatasetInterface& query,
	const SimilarityCriteria& similarityCriteria,
	cv::Mat& output) const
{
	return this->generateDiagonalMatrix(reference, query, NullSalienceMask(), similarityCriteria, output);
}

/**
 * Calculate the percentage of best matching reference images that fall within a particualar distance
 * of the query image. Use the diagonal matrix to avoid recalculating the image difference,
 * but it sill requires the datasets because we need the ground truth for each image.
 */
float PlaceRecognition::recalculatePerformance(const ImageDatasetInterface& reference, const ImageDatasetInterface& query, const cv::Mat& diagonalMatrix, const SimilarityCriteria& similarityCriteria) const
{
	float similarMatchCount = 0.0f;

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

			float score = diagonalMatrix.at<float>(i, j);
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
