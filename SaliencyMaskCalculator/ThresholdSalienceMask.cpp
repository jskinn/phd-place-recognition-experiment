/*
* ThresholdSalienceMask.cpp
*
*  Created on: 28 Jan 2016
*      Author: john
*/

#include "stdafx.h"
#include "ThresholdSalienceMask.h"

ThresholdSalienceMask::ThresholdSalienceMask(cv::Mat& maskImage) : maskImage(maskImage)
{
}

ThresholdSalienceMask::~ThresholdSalienceMask()
{
}

void ThresholdSalienceMask::applyMask(cv::Mat& image) const
{

}