/*
* ThresholdSalienceMask.h
*
*  Created on: 28 Jan 2016
*      Author: john
*/

#ifndef THRESHOLDSALIENCEMASK_H_
#define THRESHOLDSALIENCEMASK_H_

#include "SalienceMaskInterface.h"

/**
 * A salience mask that includes only the pixels within the top percentage of salience.
 */
class ThresholdSalienceMask : public SalienceMaskInterface
{
public:
	ThresholdSalienceMask(cv::Mat& maskImage, float thresholdFraction);
	virtual ~ThresholdSalienceMask();

	virtual int getNumberOfRemovedPixels() const;
	virtual void applyMask(cv::Mat& image) const;

private:
	cv::Mat mask;
	int removedPixelCount;
};

#endif /* THRESHOLDSALIENCEMASK_H_ */

