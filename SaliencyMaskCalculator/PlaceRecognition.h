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
#include "ImageMatcherInterface.h"
#include "DatasetImage.h"
#include "SimilarityCriteria.h"

class PlaceRecognition
{
public:
	PlaceRecognition();
	~PlaceRecognition();

	virtual float generateDiagonalMatrix(
		const ImageDatasetInterface& reference,
		const ImageDatasetInterface& query,
		const ImageMatcherInterface& imageMatcher,
		const SimilarityCriteria& similarityCriteria,
		cv::Mat& output) const;

	virtual float recalculatePerformance(
		const ImageDatasetInterface& reference,
		const ImageDatasetInterface& query,
		const cv::Mat& diagonalMatrix,
		const SimilarityCriteria& similarityCriteria) const;
};

#endif /* PLACERECOGNITION_H_ */