/*
* SalienceMaskInterface.h
*
*  Created on: 28 Jan 2016
*      Author: john
*/


#ifndef SALIENCEMASKINTERFACE_H_
#define SALIENCEMASKINTERFACE_H_

#include <opencv2/core/core.hpp>

class SalienceMaskInterface
{
public:
	virtual ~SalienceMaskInterface() {};

	virtual int getNumberOfRemovedPixels() const = 0;
	virtual void applyMask(cv::Mat& image) const = 0;
};

#endif /* SALIENCEMASKINTERFACE_H_ */
