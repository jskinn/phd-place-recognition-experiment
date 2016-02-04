/*
* LogicalSalienceMask.h
*
*  Created on: 4 Feb 2016
*      Author: john
*/

#ifndef LOGICALSALIENCEMASK_H_
#define LOGICALSALIENCEMASK_H_

#include <opencv2/core/core.hpp>
#include "SalienceMaskInterface.h"

class LogicalSalienceMask : public SalienceMaskInterface
{
public:
	LogicalSalienceMask(cv::Mat averageSame, cv::Mat averageDifferent);
	~LogicalSalienceMask();

	virtual int getNumberOfRemovedPixels() const;
	virtual void applyMask(cv::Mat& image) const;
};

#endif /* LOGICALSALIENCEMASK_H_ */
