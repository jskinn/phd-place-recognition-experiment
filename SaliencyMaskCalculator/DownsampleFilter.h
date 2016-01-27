/*
* DownsampleFilter.h
*
*  Created on: 26 Jan 2016
*      Author: john
*/

#ifndef DOWNSAMPLEFILTER_H_
#define DOWNSAMPLEFILTER_H_


#include "ImageFilterInterface.h"

class DownsampleFilter : public ImageFilterInterface {
public:
	DownsampleFilter(int rows, int cols);
	virtual ~DownsampleFilter();

	virtual void filterImage(cv::Mat& image) const;
private:
	int rows;
	int cols;
};

#endif /* DOWNSAMPLEFILTER_H_ */
