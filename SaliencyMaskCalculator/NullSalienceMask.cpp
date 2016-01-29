/*
* NullSalienceMask.cpp
*
*  Created on: 28 Jan 2016
*      Author: john
*/

#include "stdafx.h"
#include "NullSalienceMask.h"

NullSalienceMask::NullSalienceMask()
{
}

NullSalienceMask::~NullSalienceMask()
{
}

int NullSalienceMask::getNumberOfRemovedPixels() const
{
	return 0;
}

void NullSalienceMask::applyMask(cv::Mat& image) const
{

}
