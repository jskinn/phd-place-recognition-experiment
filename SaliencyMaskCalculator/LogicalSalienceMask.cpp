/*
* LogicalSalienceMask.cpp
*
*  Created on: 4 Feb 2016
*      Author: john
*/

#include "stdafx.h"
#include "LogicalSalienceMask.h"


LogicalSalienceMask::LogicalSalienceMask(cv::Mat averageSame, cv::Mat averageDifferent)
{
}


LogicalSalienceMask::~LogicalSalienceMask()
{
}

int LogicalSalienceMask::getNumberOfRemovedPixels() const
{
	// TODO: this will need to change to something more general purpose, possibly as part of the whole workflow around scoring image pairs changing.
	return 0;
}

void LogicalSalienceMask::applyMask(cv::Mat& image) const
{

}
