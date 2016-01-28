/*
* SalienceMask.h
*
*  Created on: 28 Jan 2016
*      Author: john
*/


#ifndef SALIENCEMASK_H_
#define SALIENCEMASK_H_

#include <opencv2/core/core.hpp>

class SalienceMask
{
public:
	SalienceMask(cv::Mat& maskImage);
	virtual ~SalienceMask();

private:
	cv::Mat maskImage;
};

#endif /* SALIENCEMASK_H_ */
