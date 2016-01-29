/*
* PlaceRecognition.cpp
*
*  Created on: 26 Jan 2016
*      Author: john
*/

#include "stdafx.h"
#include <algorithm>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "ImageFilterInterface.h"
#include "NullSalienceMask.h"
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
*/
void PlaceRecognition::generateDiagonalMatrix(
	const ImageDatasetInterface& reference,
	const ImageDatasetInterface& query,
	SalienceMaskInterface& salienceMask,
	cv::Mat& output) const
{
	output = cv::Mat::zeros(reference.count(), query.count(), CV_32FC1);

	for (int i = 0; i < query.count(); ++i) {
		// Load the query image and apply filters
		cv::Mat queryImage = query.get(i);
		if (queryImage.empty()) {
			continue;
		}

		for (int j = 0; j < reference.count(); ++j) {
			// Load the reference image and apply filters
			cv::Mat referenceImage = reference.get(j);
			if (referenceImage.empty()) {
				continue;
			}

			// Take the absolute difference between the reference and query images.
			cv::Mat diffImage;
			cv::absdiff(referenceImage, queryImage, diffImage);
			diffImage.convertTo(diffImage, CV_32FC1, 1 / 255.0);

			// Mask the image throug the salience mask
			salienceMask.applyMask(diffImage);

			cv::Scalar sum = cv::sum(diffImage);
			int numPixels = (diffImage.rows * diffImage.cols) - salienceMask.getNumberOfRemovedPixels();
			output.at<float>(i,j) = (float) sum[0] / numPixels;
		}
	}
}

void PlaceRecognition::generateDiagonalMatrix(
	const ImageDatasetInterface& reference,
	const ImageDatasetInterface& query,
	cv::Mat& output) const
{
	this->generateDiagonalMatrix(reference, query, NullSalienceMask(), output);
}
