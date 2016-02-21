/*
* SimpleComparisonMask.cpp
*
*  Created on: 5 Feb 2016
*      Author: john
*/

#include "stdafx.h"
#include "SimpleComparisonMask.h"
#include <opencv2/highgui/highgui.hpp>

const float SimpleComparisonMask::DIFFERENT_WEIGHTING = 0.5f;
const float SimpleComparisonMask::SAME_WEIGHTING = 0.5f;

SimpleComparisonMask::SimpleComparisonMask(cv::Mat averageSame, cv::Mat averageDifferent, float salientFraction, std::string outputDebugDir) :
	salientPixels(SimpleComparisonMask::findMostSalientPixels(averageSame, averageDifferent, salientFraction)),
	//averageSame(averageSame.mul(salientPixels)),
	//averageDifferent(averageDifferent.mul(salientPixels)),
	numberOfSalientPixels(std::ceil(salientFraction * averageSame.rows * averageSame.cols))
{
	assert(salientFraction >= 0.0f);
	assert(salientFraction <= 1.0f);
	assert(numberOfSalientPixels > 0);

	averageSame = averageSame.mul(salientPixels);
	averageDifferent = averageDifferent.mul(averageDifferent);
	sameMinusDifferent = averageSame - averageDifferent;
	samePlusDifferent = averageSame + averageDifferent;

	cv::Mat outputImage;
	salientPixels.convertTo(outputImage, CV_8UC1, 255.0);
	cv::imwrite(outputDebugDir + "\\most salient pixels.png", outputImage);
}


SimpleComparisonMask::~SimpleComparisonMask()
{
}

float SimpleComparisonMask::matchImages(const cv::Mat& imageA, const cv::Mat& imageB) const
{
	// Compute the difference image
	cv::Mat diffImage;
	cv::absdiff(imageA, imageB, diffImage);
	diffImage.convertTo(diffImage, CV_32FC1, 1 / 255.0);
	diffImage = diffImage.mul(salientPixels);

	// Calculate the score for the image, based on the difference and the mask information
	// Assume the diff image and average images are the same size :S. I can always call resize if this becomes a problem.
	// TODO: Not needed any more
	//cv::Mat diff1 = diffImage - averageSame;
	//cv::Mat diff2 = diffImage - averageDifferent;

	// Equation is (image - avgSame)^2 - (image - averageDifferent)^2
	// the equation simplifies to (averageSame - averageDifferent) * (-2 * diffImage + averageSame + averageDifferent)
	// This equation should form a line crossing 0 half way between the averageSame value and the averageDifferentValue
	// The value will be negative closer to averageSame, and positive closer to averageDifferent
	// Maximum range is (-1,1), with a smaller slope when averageSame and averageDifferent are closer together.
	// I could add scaling terms to each of the square differences,
	// which adjust the overall scale and introduce some curvature, but I' haven't done that yet.
	//float score = cv::sum(diff1.mul(diff1) - diff2.mul(diff2))[0];	// Old way with too much computation
	float score = cv::sum(sameMinusDifferent.mul(-2 * diffImage + samePlusDifferent))[0];
	// Normalize the score back to the range 0-1.
	score = (score / (2.0f * numberOfSalientPixels)) + 0.5f;
	assert(score >= 0.0f);
	assert(score <= 1.0f);
	return score;
}

/**
* Find the most salient pixels.
* I've pulled this out so I can test it (if I get to that at all).
* I'm not sure if this is best as returning a vector or taking a reference to one as a parameter.
*/
cv::Mat SimpleComparisonMask::findMostSalientPixels(const cv::Mat& averageSame, const cv::Mat& averageDifferent, const float salientFraction)
{
	// Make a vector of pairs, each par containing a Point that is the pixel location and 
	int numberOfSalientPixels = (int)std::ceil(salientFraction * averageSame.rows * averageSame.cols);
	std::vector<cv::Point> chosenPoints = std::vector<cv::Point>(numberOfSalientPixels);
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
				cv::Point swapPoint = chosenPoints[index];
				salientPixelScore[index] = currentSalience;
				chosenPoints[index] = currentPoint;
				currentSalience = swapSalience;
				currentPoint = swapPoint;
			}
		}
	}

	// Convert the chosen points into a mask.
	// While we're at it, check that the chosen points are in descending order of salience.
	float runningSalience = 0.0f;
	cv::Mat outputMask = cv::Mat::zeros(salienceImage.rows, salienceImage.cols, CV_32FC1);
	for (int i = 0; i < chosenPoints.size(); ++i) {
		outputMask.at<float>(chosenPoints[i]) = 1.0f;

		// Check the salience of the chosen points.
		float salience = salienceImage.at<float>(chosenPoints[i]);
		if (i > 0) {
			assert(salience <= runningSalience);
		}
		runningSalience = salience;
	}

	return outputMask;
}
