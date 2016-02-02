/*
* SalienceMaskGeneratorInterface.h
*
*  Created on: 28 Jan 2016
*      Author: john
*/


#ifndef SALIENCEMASKGENERATORINTERFACE_H_
#define SALIENCEMASKGENERATORINTERFACE_H_

#include <list>
#include <opencv2/core/core.hpp>
#include "ImageFilterInterface.h"
#include "ImageDatasetInterface.h"

class SalienceMaskGeneratorInterface
{
public:
	virtual	~SalienceMaskGeneratorInterface() {} // Empty destructor for interface

	/**
	 * Generate a salience mask.
	 */
	virtual void generateSalienceMask(
		const ImageDatasetInterface& reference,
		const ImageDatasetInterface& query,
		cv::Mat& outputMask) const = 0;
};

#endif /* SALIENCEMASKGENERATORINTERFACE_H_ */
