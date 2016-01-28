/*
* AverageDifferenceMaskGenerator.h
*
*  Created on: 28 Jan 2016
*      Author: john
*/


#ifndef AVERAGEDIFFERENCEMASKGENERATOR_H_
#define AVERAGEDIFFERENCEMASKGENERATOR_H_

#include "SalienceMaskGeneratorInterface.h"

class AverageDifferenceMaskGenerator : public SalienceMaskGeneratorInterface
{
public:
	AverageDifferenceMaskGenerator();
	virtual ~AverageDifferenceMaskGenerator();

	virtual void generateSalienceMask(
		const ImageDatasetInterface& reference,
		const ImageDatasetInterface& query,
		cv::Mat& outputMask) const;

	virtual void generateSalienceMask(
		const ImageDatasetInterface& reference,
		const ImageDatasetInterface& query,
		std::list<ImageFilterInterface*>& filters,
		cv::Mat& outputMask) const;
};

#endif /* AVERAGEDIFFERENCEMASKGENERATOR_H_ */
