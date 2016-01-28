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
#include "SalienceMask.h"

class SalienceMaskGeneratorInterface
{
public:
	virtual	~SalienceMaskGeneratorInterface() {} // Empty destructor for interface

	/**
	 * Generate a salience mask.
	 * Overloaded to not have any filters,
	 * See the method below
	 */
	virtual SalienceMask generateSalienceMask(
		const ImageDatasetInterface& reference,
		const ImageDatasetInterface& query) const = 0;

	/**
	 * Generate a salience mask.
	 * 
	 */
	virtual SalienceMask generateSalienceMask(
		const ImageDatasetInterface& reference,
		const ImageDatasetInterface& query,
		std::list<ImageFilterInterface*>& filters) const = 0;
};

#endif /* SALIENCEMASKGENERATORINTERFACE_H_ */
