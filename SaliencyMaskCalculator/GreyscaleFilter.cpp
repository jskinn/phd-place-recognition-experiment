/*
* GreyscaleFilter.cpp
*
*  Created on: 26 Jan 2016
*      Author: john
*/

#include "stdafx.h"
#include <opencv2/imgproc/imgproc.hpp>

#include "GreyscaleFilter.h"

GreyscaleFilter::GreyscaleFilter() {
	// TODO Auto-generated constructor stub

}

GreyscaleFilter::~GreyscaleFilter() {
	// TODO Auto-generated destructor stub
}

void GreyscaleFilter::filterImage(cv::Mat& image) const {
	cv::cvtColor(image, image, cv::COLOR_RGB2GRAY);
}
