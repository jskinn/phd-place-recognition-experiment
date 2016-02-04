/*
* SumOfAbsoluteDifferencesMatcher.h
*
*  Created on: 4 Feb 2016
*      Author: john
*/

#ifndef SUMOFABSOLUTEDIFFERENCESMATCHER_H_
#define SUMOFABSOLUTEDIFFERENCESMATCHER_H_

#include "ImageMatcherInterface.h"

class SumOfAbsoluteDifferencesMatcher : public ImageMatcherInterface
{
public:
	SumOfAbsoluteDifferencesMatcher();
	virtual ~SumOfAbsoluteDifferencesMatcher();

	virtual float matchImages(const cv::Mat& imageA, const cv::Mat& imageB) const;
};

#endif /* SUMOFABSOLUTEDIFFERENCESMATCHER_H_ */
