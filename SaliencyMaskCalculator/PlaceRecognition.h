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
#include "SalienceMaskInterface.h"
#include "DatasetImage.h"

class PlaceRecognition
{
public:
	PlaceRecognition();
	~PlaceRecognition();

	virtual void generateDiagonalMatrix(
		const ImageDatasetInterface& reference,
		const ImageDatasetInterface& query,
		cv::Mat& output) const;

	virtual void generateDiagonalMatrix(
		const ImageDatasetInterface& reference,
		const ImageDatasetInterface& query,
		SalienceMaskInterface& salienceMask,
		cv::Mat& output) const;

	virtual float measurePerformance(cv::Mat& diagonalMatrix, int similarityWindow = 1) const;
};

#endif /* PLACERECOGNITION_H_ */