/*
* ThresholdSalienceMask.cpp
*
*  Created on: 28 Jan 2016
*      Author: john
*/

#include "stdafx.h"
#include "ThresholdSalienceMask.h"
#include <opencv2/imgproc/imgproc.hpp>

#include <opencv2/highgui/highgui.hpp>

ThresholdSalienceMask::ThresholdSalienceMask(cv::Mat& maskImage, float thresholdFraction, std::string outputDebugDir) :
	mask(findMostSalientPixels(maskImage, thresholdFraction)),
	numberOfSalientPixels(std::ceil(thresholdFraction * maskImage.rows * maskImage.cols))
{
	/*double min, max;
	cv::minMaxLoc(maskImage, &min, &max);
	double threshold = min + thresholdFraction * (max - min);
	cv::threshold(maskImage, this->mask, threshold, 1.0f, cv::THRESH_BINARY);
	this->removedPixelCount = (maskImage.rows * maskImage.cols) - cv::countNonZero(this->mask);*/

	cv::Mat outputImage;
	this->mask.convertTo(outputImage, CV_8UC1, 255.0);
	cv::imwrite(outputDebugDir + "\\effective mask.png", outputImage);
	/*cv::namedWindow("Display window", cv::WINDOW_AUTOSIZE); // Create a window for display.
	cv::imshow("Display window", this->mask);               // Show our image inside it.
	cv::waitKey(0);                                         // Wait for a keystroke in the window*/
}

ThresholdSalienceMask::~ThresholdSalienceMask()
{
}

float ThresholdSalienceMask::matchImages(const cv::Mat& imageA, const cv::Mat& imageB) const
{
	cv::Mat diffImage;
	cv::absdiff(imageA, imageB, diffImage);
	diffImage.convertTo(diffImage, CV_32FC1, 1 / 255.0);

	// Apply the salience mask
	diffImage = diffImage.mul(this->mask);

	// Compute the similarity score based on the sum of absolute differences between the images.
	cv::Scalar sum = cv::sum(diffImage);
	return (float)sum[0] / cv::countNonZero(this->mask);
}


/**
* Find the most salient pixels.
* I've pulled this out so I can test it (if I get to that at all).
* I'm not sure if this is best as returning a vector or taking a reference to one as a parameter.
*/
cv::Mat ThresholdSalienceMask::findMostSalientPixels(const cv::Mat& salienceImage, const float salientFraction) const
{
	// Make a vector of pairs, each par containing a Point that is the pixel location and 
	int numberOfSalientPixels = (int)std::ceil(salientFraction * salienceImage.rows * salienceImage.cols);
	std::vector<cv::Point> chosenPoints = std::vector<cv::Point>(numberOfSalientPixels);
	std::vector<float> salientPixelScore = std::vector<float>(numberOfSalientPixels, 0.0f);

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