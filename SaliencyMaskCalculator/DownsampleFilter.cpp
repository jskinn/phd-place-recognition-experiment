/*
* DownsampleFilter.cpp
*
*  Created on: 26 Jan 2016
*      Author: john
*/

#include "stdafx.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "DownsampleFilter.h"

DownsampleFilter::DownsampleFilter(int rows, int cols) :
	rows(rows),
	cols(cols)
{
}

DownsampleFilter::~DownsampleFilter() {
}

void DownsampleFilter::filterImage(cv::Mat& image) const {
	cv::resize(image, image, cv::Size(this->rows, this->cols));
}

