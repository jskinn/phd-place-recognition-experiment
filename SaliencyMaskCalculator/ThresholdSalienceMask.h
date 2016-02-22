/*
* ThresholdSalienceMask.h
*
*  Created on: 28 Jan 2016
*      Author: john
*/

#ifndef THRESHOLDSALIENCEMASK_H_
#define THRESHOLDSALIENCEMASK_H_

#include "ImageMatcherInterface.h"

/**
 * A salience mask that includes only the pixels within the top percentage of salience.
 */
class ThresholdSalienceMask : public ImageMatcherInterface
{
public:
	ThresholdSalienceMask(cv::Mat& maskImage, float thresholdFraction, std::string outputDebugDir);
	virtual ~ThresholdSalienceMask();

	virtual float matchImages(const cv::Mat& imageA, const cv::Mat& imageB) const;

private:
	cv::Mat mask;
	float numberOfSalientPixels;

	cv::Mat ThresholdSalienceMask::findMostSalientPixels(const cv::Mat& salienceImage, const float salientFraction) const;
};

#endif /* THRESHOLDSALIENCEMASK_H_ */

