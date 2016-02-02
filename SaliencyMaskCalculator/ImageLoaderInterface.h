/*
* ImageLoaderInterface.h
*
*  Created on: 27 Jan 2016
*      Author: john
*/

#ifndef IMAGELOADERINTERFACE_H_
#define IMAGELOADERINTERFACE_H_

#include "DatasetImage.h"

class ImageLoaderInterface
{
public:
	virtual ~ImageLoaderInterface() {};

	/**
	 * Get the number of images loaded by this loader.
	 * Used to determine the amount of memory you need to pass to loadImages.
	 */
	virtual int getCount() = 0;

	/**
	 * Load all the images into the provided memory.
	 * Treats the argument as an array, and loads images into the provided vector
	 * from index indexOffset to indexOffset + getCount().
	 * 
	 * Must only modify the provided destination vector over the specified
	 * index range, don't touch outside that.
	 */
	virtual void loadImages(std::vector<DatasetImage*>& destination, int indexOffset) = 0;
};

#endif /* IMAGELOADERINTERFACE_H_ */

