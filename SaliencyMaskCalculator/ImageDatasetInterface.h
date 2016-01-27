/*
* ImageDatasetInterface.h
*
*  Created on: 27 Jan 2016
*      Author: john
*/

#ifndef IMAGEDATASETINTERFACE_H_
#define IMAGEDATASETINTERFACE_H_

#include <opencv2/core/core.hpp>

class ImageDatasetInterface
{
public:
	~ImageDatasetInterface() {};

	virtual int count() const = 0;
	virtual cv::Mat& get(int index) const = 0;
};

#endif /* IMAGEDATASETINTERFACE_H_ */