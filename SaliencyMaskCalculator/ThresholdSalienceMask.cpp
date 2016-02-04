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

int ThresholdSalienceMask::getNumberOfRemovedPixels() const
{
	return this->removedPixelCount;
}

void ThresholdSalienceMask::applyMask(cv::Mat& image) const
{
	image = image.mul(this->mask);
}