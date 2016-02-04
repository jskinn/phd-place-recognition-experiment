/*
* ImageMatcherInterface.h
*
*  Created on: 4 Feb 2016
*      Author: john
*/

#ifndef IMAGEMATCHERINTERFACE_H_
#define IMAGEMATCHERINTERFACE_H_

#include <opencv2/core/core.hpp>

class ImageMatcherInterface
{
public:
	virtual ~ImageMatcherInterface() {};

	/**
	 * Compare two images and return a match score.
	 * 
	 * As a consequence of the comparison, this should be associative, ie: matchImages(A, B) == matchImages(B, A);
	 * For most of this experiment, this should be based on an absolute difference between the images.
	 */
	virtual float matchImages(const cv::Mat& imageA, const cv::Mat& imageB) const = 0;
};

#endif /* IMAGEMATCHERINTERFACE_H_ */
