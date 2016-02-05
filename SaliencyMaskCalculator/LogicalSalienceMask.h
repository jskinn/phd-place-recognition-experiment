/*
* LogicalSalienceMask.h
*
*  Created on: 4 Feb 2016
*      Author: john
*/

#ifndef LOGICALSALIENCEMASK_H_
#define LOGICALSALIENCEMASK_H_

#include <vector>
#include <utility>
#include <opencv2/core/core.hpp>
#include "ImageMatcherInterface.h"

class LogicalSalienceMask : public ImageMatcherInterface
{
public:
	LogicalSalienceMask(cv::Mat averageSame, cv::Mat averageDifferent, float salientFraction);
	~LogicalSalienceMask();

	virtual float matchImages(const cv::Mat& imageA, const cv::Mat& imageB) const;

	static const float DIFFERENT_WEIGHTING;
	static const float SAME_WEIGHTING;

	static std::vector<cv::Point> findMostSalientPixels(const cv::Mat& averageSame, const cv::Mat& averageDifferent, const float salientFraction);

private:
	cv::Mat averageSame;
	cv::Mat averageDifferent;

	std::vector<cv::Point> salientPixels;
};

#endif /* LOGICALSALIENCEMASK_H_ */
