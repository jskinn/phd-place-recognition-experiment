#include "stdafx.h"
#include "WeightingSalienceMask.h"

#include <opencv2/highgui/highgui.hpp>

WeightingSalienceMask::WeightingSalienceMask(cv::Mat& maskImage)
{
	cv::normalize(maskImage, this->mask, 0.0, 1.0, cv::NORM_MINMAX);
	this->removedPixelCount = (maskImage.rows * maskImage.cols) - cv::countNonZero(this->mask);

	cv::namedWindow("Display window", cv::WINDOW_AUTOSIZE); // Create a window for display.
	cv::imshow("Display window", this->mask);               // Show our image inside it.
	cv::waitKey(0);                                         // Wait for a keystroke in the window
}


WeightingSalienceMask::~WeightingSalienceMask()
{
}

int WeightingSalienceMask::getNumberOfRemovedPixels() const
{
	return this->removedPixelCount;
}

void WeightingSalienceMask::applyMask(cv::Mat& image) const
{
	image = image.mul(this->mask);
}