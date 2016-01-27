/*
* GreyscaleFilter.h
*
*  Created on: 26 Jan 2016
*      Author: john
*/

#ifndef GREYSCALEFILTER_H_
#define GREYSCALEFILTER_H_

#include "ImageFilterInterface.h"

class GreyscaleFilter : public ImageFilterInterface {
public:
	GreyscaleFilter();
	virtual ~GreyscaleFilter();

	virtual void filterImage(cv::Mat& image) const;
};

#endif /* GREYSCALEFILTER_H_ */
