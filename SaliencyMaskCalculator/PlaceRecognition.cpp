/*
* PlaceRecognition.cpp
*
*  Created on: 26 Jan 2016
*      Author: john
*/

#include "stdafx.h"
#include <algorithm>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "ImageFilterInterface.h"
#include "PlaceRecognition.h"

PlaceRecognition::PlaceRecognition()
{
}


PlaceRecognition::~PlaceRecognition()
{
}

/**
* Generate a diagonal matrix comparing each query image to all the reference images,
* and measuring the similar matrix.
* We want the matrix to have a bright diagonal and low values everywhere else.
*
* TODO: use the salience mask somehow
*/
void PlaceRecognition::generateDiagonalMatrix(
	const ImageDatasetInterface& reference,
	const ImageDatasetInterface& query,
	cv::Mat& output,
	std::list<ImageFilterInterface*> filters,
	cv::Mat& salienceMask) const
{
	int i, j;
	cv::Mat referenceImage, queryImage, diffImage;
	output = cv::Mat::zeros(reference.count(), query.count(), CV_32F);

	for (i = 0; i < query.count(); ++i) {
		// Load the query image and apply filters
		queryImage = query.get(i);
		if (queryImage.empty()) {
			continue;
		}
		ImageFilterInterface::applyFilters(queryImage, filters);

		for (j = 0; j < reference.count(); ++j) {
			// Load the reference image and apply filters
			referenceImage = reference.get(i);
			if (referenceImage.empty()) {
				continue;
			}
			ImageFilterInterface::applyFilters(referenceImage, filters);

			cv::absdiff(referenceImage, queryImage, diffImage);
			output.at<cv::Scalar>(i,j) = cv::sum(diffImage);
		}
	}
}

/**
* Overload generate salience mask, without any image filters.
*/
void PlaceRecognition::generateSalienceMask(
	const ImageDatasetInterface& reference,
	const ImageDatasetInterface& query,
	cv::Mat& mask) const
{
	std::list<ImageFilterInterface*> filters;
	this->generateSalienceMask(reference, query, mask, filters);
}

/**
* Generate a salience mask, by comparing
* TODO: This needs tweaking, it's assuming the indexes in the datasets match ground truth,
* It's also generating a bad salience mask.
*/
void PlaceRecognition::generateSalienceMask(
	const ImageDatasetInterface& reference,
	const ImageDatasetInterface& query,
	cv::Mat& mask,
	std::list<ImageFilterInterface*>& filters) const
{
	int datasetSize = std::min(reference.count(), query.count());
	double sadSum(0.0);
	int index;
	cv::Mat referenceImage, queryImage, diffImage, avgDiff;

	for (index = 0; index < datasetSize; ++index) {
		// Read images and validate input. Move to next pair if there's as problem.
		referenceImage = reference.get(index);
		queryImage = query.get(index);
		if (referenceImage.empty() || queryImage.empty()) {
			continue;
		}

		// Apply all the filters to the images
		ImageFilterInterface::applyFilters(referenceImage, filters);
		ImageFilterInterface::applyFilters(queryImage, filters);

		cv::absdiff(referenceImage, queryImage, diffImage);
		if (index == 0) {
			diffImage.convertTo(avgDiff, CV_32F);
		} else {
			cv::add(avgDiff, diffImage, avgDiff, cv::noArray(), CV_32F);
		}
	}
	mask = avgDiff * (1.0 / datasetSize);
}