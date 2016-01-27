/*
* PlaceRecognition.h
*
*  Created on: 27 Jan 2016
*      Author: john
*/


#ifndef PLACERECOGNITION_H_
#define PLACERECOGNITION_H_

#include <list>
#include <opencv2/core/core.hpp>
#include "ImageFilterInterface.h"
#include "ImageDatasetInterface.h"

class PlaceRecognition
{
public:
	PlaceRecognition();
	~PlaceRecognition();

	virtual void generateDiagonalMatrix(
		const ImageDatasetInterface& reference,
		const ImageDatasetInterface& query,
		cv::Mat& output,
		std::list<ImageFilterInterface*> filters = std::list<ImageFilterInterface*>(),
		cv::Mat& salienceMask = cv::Mat()) const;

	virtual void generateSalienceMask(
		const ImageDatasetInterface& reference,
		const ImageDatasetInterface& query,
		cv::Mat& mask) const;

	virtual void generateSalienceMask(
		const ImageDatasetInterface& reference,
		const ImageDatasetInterface& query,
		cv::Mat& mask,
		std::list<ImageFilterInterface*>& filters) const;
};

#endif /* PLACERECOGNITION_H_ */