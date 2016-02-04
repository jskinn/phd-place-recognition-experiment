/*
* LogicalSalienceMask.h
*
*  Created on: 4 Feb 2016
*      Author: john
*/

#ifndef LOGICALSALIENCEMASK_H_
#define LOGICALSALIENCEMASK_H_

#include <opencv2/core/core.hpp>
#include "ImageMatcherInterface.h"

class LogicalSalienceMask : public ImageMatcherInterface
{
public:
	LogicalSalienceMask(cv::Mat averageSame, cv::Mat averageDifferent);
	~LogicalSalienceMask();

	virtual float matchImages(const cv::Mat& imageA, const cv::Mat& imageB) const;
};

#endif /* LOGICALSALIENCEMASK_H_ */
