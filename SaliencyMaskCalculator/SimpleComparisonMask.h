/*
* SimpleComparisonMask.h
*
*  Created on: 5 Feb 2016
*      Author: john
*/

#ifndef SIMPLECOMPARISONMASK_H_
#define SIMPLECOMPARISONMASK_H_

#include <vector>
#include "ImageMatcherInterface.h"

class SimpleComparisonMask : public ImageMatcherInterface
{
public:
	SimpleComparisonMask(cv::Mat averageSame, cv::Mat averageDifferent, float salientFraction);
	virtual ~SimpleComparisonMask();

	virtual float matchImages(const cv::Mat& imageA, const cv::Mat& imageB) const;

	static const float DIFFERENT_WEIGHTING;
	static const float SAME_WEIGHTING;

	static cv::Mat findMostSalientPixels(const cv::Mat& averageSame, const cv::Mat& averageDifferent, const float salientFraction);

private:
	cv::Mat salientPixels;
	cv::Mat averageSame;
	cv::Mat averageDifferent;
	float numberOfSalientPixels;
};

#endif /* SIMPLECOMPARISONMASK_H_ */
