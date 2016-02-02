/*
* LinearTraverseLoader.h
*
*  Created on: 2 Feb 2016
*      Author: john
*/

#ifndef LINEARTRAVERSELOADER_H_
#define LINEARTRAVERSELOADER_H_

#include <string>
#include <list>
#include "ImageFilterInterface.h"
#include "ImageLoaderInterface.h"

class LinearTraverseLoader :
	public ImageLoaderInterface
{
public:
	LinearTraverseLoader(
		cv::Vec3d startLocation,
		cv::Vec3d finalLocation,
		cv::Vec3d orientation,
		
		std::string filenamePrefix,
		std::string filenameSuffix,
		int imageCount,
		int initialIndex,
		int indexStep,
		int indexPad,
		const std::list<ImageFilterInterface*>& filters = std::list<ImageFilterInterface*>());

	virtual ~LinearTraverseLoader();

	virtual int getCount();

	virtual void loadImages(std::vector<DatasetImage*>& destination, int indexOffset);

private:
	// Ground truth parameters
	cv::Vec3d startLocation;
	cv::Vec3d locationStep;
	cv::Vec3d orientation;

	// File loading parameters
	std::string filenamePrefix;
	std::string filenameSuffix;
	int imageCount;
	int initialIndex;
	int indexStep;
	int indexPad;
	const std::list<ImageFilterInterface*>& filters;
};

#endif /* LINEARTRAVERSELOADER_H_ */