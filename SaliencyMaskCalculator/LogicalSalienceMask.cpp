/*
* LogicalSalienceMask.cpp
*
*  Created on: 4 Feb 2016
*      Author: john
*/

#include <assert.h>
#include "stdafx.h"
#include "LogicalSalienceMask.h"

const float LogicalSalienceMask::DIFFERENT_WEIGHTING = 1.0f;
const float LogicalSalienceMask::SAME_WEIGHTING = 1.0f;

LogicalSalienceMask::LogicalSalienceMask(cv::Mat averageSame, cv::Mat averageDifferent, float salientFraction) :
	averageSame(averageSame),
	averageDifferent(averageDifferent),
	salientPixels(LogicalSalienceMask::findMostSalientPixels(averageSame, averageDifferent, salientFraction))
{
}


LogicalSalienceMask::~LogicalSalienceMask()
{
}

float LogicalSalienceMask::matchImages(const cv::Mat& imageA, const cv::Mat& imageB) const
{
	// Compute the difference image
	cv::Mat diffImage;
	cv::absdiff(imageA, imageB, diffImage);
	diffImage.convertTo(diffImage, CV_32FC1, 1 / 255.0);

	// Calculate the score for the image, based on the difference and the mask information
	// Assume the diff image and average images are the same size :S. I can always call resize if this becomes a problem.
	float score = 0.0f;
	float relevantPixels = 0.0f;
	/*for (int i = 0; i < diffImage.rows; ++i) {
		for (int j = 0; j < diffImage.cols; ++j) {
			float difference = diffImage.at<float>(i, j);
			float differenceForSame = averageSame.at<float>(i, j);
			float differenceForDifferent = averageDifferent.at<float>(i, j);

			// TODO: Skip this pixel unless it's a 'relevant' pixel.

			// Use a gaussian around the expected difference for matching or not.
			// The gaussian for the differenceForDifferent increases the score if the actual difference is nearby,
			// while the gaussian for the differenceForSame increases the score unless the actual difference is nearby.
			// Expected score increase ranges 0-2, but is expected to be 1.
			score += LogicalSalienceMask::DIFFERENT_WEIGHTING * std::exp(-10.0f * (difference - differenceForDifferent)) +
				LogicalSalienceMask::SAME_WEIGHTING * (1 - std::exp(-10.0f * (difference - differenceForSame)));

			++relevantPixels;
		}
	}*/

	for (std::vector<cv::Point>::const_iterator iter = salientPixels.begin(); iter != salientPixels.end(); ++iter) {
		float difference = diffImage.at<float>(*iter);
		float differenceForSame = averageSame.at<float>(*iter);
		float differenceForDifferent = averageDifferent.at<float>(*iter);

		// TODO: Skip this pixel unless it's a 'relevant' pixel.

		// Use a gaussian around the expected difference for matching or not.
		// The gaussian for the differenceForDifferent increases the score if the actual difference is nearby,
		// while the gaussian for the differenceForSame increases the score unless the actual difference is nearby.
		// Expected score increase ranges 0-2, but is expected to be 1.
		float diff1 = difference - differenceForDifferent;
		float diff2 = difference - differenceForSame;
		score += LogicalSalienceMask::DIFFERENT_WEIGHTING * std::exp(-10.0f * diff1 * diff1) +
			LogicalSalienceMask::SAME_WEIGHTING * (1 - std::exp(-10.0f * diff2 * diff2));

		++relevantPixels;
	}

	// Compute the final score, averaged over all the relevant pixels
	assert(score < (LogicalSalienceMask::DIFFERENT_WEIGHTING + LogicalSalienceMask::SAME_WEIGHTING) * relevantPixels);
	assert(score >= 0.0f);
	return score / relevantPixels;
}

/**
 * Find the most salient pixels.
 * I've pulled this out so I can test it (if I get to that at all).
 * I'm not sure if this is best as returning a vector or taking a reference to one as a parameter.
 */
std::vector<cv::Point> LogicalSalienceMask::findMostSalientPixels(const cv::Mat& averageSame, const cv::Mat& averageDifferent, const float salientFraction)
{
	// Make a vector of pairs, each par containing a Point that is the pixel location and 
	int numberOfSalientPixels = (int)std::ceil(salientFraction * averageSame.rows * averageSame.cols);
	std::vector<cv::Point> outputPoints = std::vector<cv::Point>(numberOfSalientPixels);
	std::vector<float> salientPixelScore = std::vector<float>(numberOfSalientPixels, 0.0f);

	// The most salient pixels are those with the largest difference between the same and different average differences.
	cv::Mat salienceImage;
	cv::absdiff(averageSame, averageDifferent, salienceImage);

	// Find the highest k pixel locations
	// This custom approach should be O(nk) in the worst case, but the average case should be much better.
	// I couldn't find a standard algorithm that would 
	for (int i = 0; i < salienceImage.rows; ++i) {
		for (int j = 0; j < salienceImage.cols; ++j) {
			int index = numberOfSalientPixels;
			cv::Point currentPoint(j, i);	// Point constructor is (columns, rows)
			float currentSalience = salienceImage.at<float>(currentPoint);	// Make sure the point and salience match, to avoid index order confusion.

			// Start at the end of the list and work forward to find the lowest index where the score is less than
			// the score for the current pixel.
			// Note that if the pixel is 
			while (index > 0 && salientPixelScore[index - 1] < currentSalience) {
				--index;
			}

			// Swap all the way back down to the end of the list, with the last one falling off the bottom
			// Note that this loop won't run at all if the current salience is less than the lowest 
			for (; index < numberOfSalientPixels; ++index) {
				float swapSalience = salientPixelScore[index];
				cv::Point swapPoint = outputPoints[index];
				salientPixelScore[index] = currentSalience;
				outputPoints[index] = currentPoint;
				currentSalience = swapSalience;
				currentPoint = swapPoint;
			}
		}
	}

	return outputPoints;
}
