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
void PlaceRecognition::generateDiagonalMatrix(
	const ImageDatasetInterface& reference,
	const ImageDatasetInterface& query,
	SalienceMaskInterface& salienceMask,
	cv::Mat& output) const
{
	output = cv::Mat::zeros(reference.count(), query.count(), CV_32FC1);

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
			diffImage.convertTo(diffImage, CV_32FC1, 1 / 255.0);

			// Mask the image throug the salience mask
			salienceMask.applyMask(diffImage);

			cv::Scalar sum = cv::sum(diffImage);
			int numPixels = (diffImage.rows * diffImage.cols) - salienceMask.getNumberOfRemovedPixels();
			float matchScore = (float)sum[0] / numPixels;
			assert(matchScore >= 0.0f);
			assert(matchScore <= 1.0f);
			output.at<float>(i,j) = matchScore;
		}
	}
}

void PlaceRecognition::generateDiagonalMatrix(
	const ImageDatasetInterface& reference,
	const ImageDatasetInterface& query,
	cv::Mat& output) const
{
	this->generateDiagonalMatrix(reference, query, NullSalienceMask(), output);
}

/**
 * Measure the performance of the place recognition, as encoded in the diagonal matrix.
 * Makes the following assumptions:
 *   - The diagonal matrix index is (queryIndex, referenceIndex), so the number of rows are the number of query images and columns the number of reference images.
 *   - Matching indexes on the query and reference images indicate that the images are from the same place.
 *   - The diagonal matrix pixels are in the range 0 - 1, with lower numbers indicating a closer match.
 */
float PlaceRecognition::measurePerformance(cv::Mat& diagonalMatrix, int similarityWindow) const
{
	float correctCount = 0;
	int querySize = diagonalMatrix.rows;
	int referenceSize = diagonalMatrix.cols;

	for (int i = 0; i < querySize; ++i) {
		float bestScore = 1.0f;	// Theoretical max value for the diagonal matrix at 
		int bestIndex = -1;

		for (int j = 0; j < referenceSize; ++j) {
			float score = diagonalMatrix.at<float>(i, j);
			if (score < bestScore) {
				bestScore = score;
				bestIndex = j;
			}
		}

		if (std::abs(i - bestIndex) <= similarityWindow) {
			correctCount++;
		}
	}

	return correctCount / querySize;
}
