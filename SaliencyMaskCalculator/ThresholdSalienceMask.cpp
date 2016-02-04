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

ThresholdSalienceMask::ThresholdSalienceMask(cv::Mat& maskImage, float thresholdFraction)
{
	double min, max;
	cv::minMaxLoc(maskImage, &min, &max);
	double threshold = min + thresholdFraction * (max - min);
	cv::threshold(maskImage, this->mask, threshold, 1.0f, cv::THRESH_BINARY);
	this->removedPixelCount = (maskImage.rows * maskImage.cols) - cv::countNonZero(this->mask);

	cv::Mat outputImage;
	this->mask.convertTo(outputImage, CV_8UC1, 255.0);
	cv::imwrite("C:\\LocalUser\\Documents\\Renders\\city dataset 2016-01-21\\effective mask.png", outputImage);
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