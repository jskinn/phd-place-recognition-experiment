/*
* SumOfAbsoluteDifferencesMatcher.cpp
*
*  Created on: 4 Feb 2016
*      Author: john
*/

#include "stdafx.h"
#include "SumOfAbsoluteDifferencesMatcher.h"


SumOfAbsoluteDifferencesMatcher::SumOfAbsoluteDifferencesMatcher()
{
}


SumOfAbsoluteDifferencesMatcher::~SumOfAbsoluteDifferencesMatcher()
{
}

float SumOfAbsoluteDifferencesMatcher::matchImages(const cv::Mat& imageA, const cv::Mat& imageB) const
{
	cv::Mat diffImage;
	cv::absdiff(imageA, imageB, diffImage);
	diffImage.convertTo(diffImage, CV_32FC1, 1 / 255.0);

	// Compute the similarity score based on the sum of absolute differences between the images.
	cv::Scalar sum = cv::sum(diffImage);
	return (float)sum[0] / (diffImage.rows * diffImage.cols);
}
