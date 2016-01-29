/*
* ThresholdSalienceMask.cpp
*
*  Created on: 28 Jan 2016
*      Author: john
*/

#include "stdafx.h"
#include "ThresholdSalienceMask.h"
#include <opencv2/imgproc/imgproc.hpp>

ThresholdSalienceMask::ThresholdSalienceMask(cv::Mat& maskImage, float thresholdFraction)
{
	double min, max;
	cv::minMaxLoc(maskImage, &min, &max);
	double threshold = min + thresholdFraction * (max - min);
	cv::threshold(maskImage, this->mask, threshold, 1.0f, cv::THRESH_BINARY);
	this->removedPixelCount = (maskImage.rows * maskImage.cols) - cv::countNonZero(this->mask);
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