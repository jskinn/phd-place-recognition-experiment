/*
* ImageDatasetInterface.h
*
*  Created on: 27 Jan 2016
*      Author: john
*/

#ifndef IMAGEDATASETINTERFACE_H_
#define IMAGEDATASETINTERFACE_H_

#include "DatasetImage.h"

class ImageDatasetInterface
{
public:
	virtual ~ImageDatasetInterface() {};

	virtual int count() const = 0;
	virtual DatasetImage& get(int index) const = 0;
};

#endif /* IMAGEDATASETINTERFACE_H_ */