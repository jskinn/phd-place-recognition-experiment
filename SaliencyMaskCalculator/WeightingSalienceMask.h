/*
* WeightingSalienceMask.h
*
*  Created on: 29 Jan 2016
*      Author: john
*/

#ifndef WEIGHTINGSALIENCEMASK_H_
#define WEIGHTINGSALIENCEMASK_H_

#include "SalienceMaskInterface.h"

class WeightingSalienceMask : public SalienceMaskInterface
{
public:
	WeightingSalienceMask(cv::Mat& maskImage);
	~WeightingSalienceMask();
	
	virtual int getNumberOfRemovedPixels() const;
	virtual void applyMask(cv::Mat& image) const;

private:
	cv::Mat mask;
	int removedPixelCount;
};

#endif /* WEIGHTINGSALIENCEMASK_H_ */