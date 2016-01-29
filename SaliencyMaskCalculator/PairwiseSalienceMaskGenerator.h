/*
* PairwiseSalienceMaskGenerator.h
*
*  Created on: 29 Jan 2016
*      Author: john
*/

#ifndef PAIRWISESALIENCEMASKGENERATOR_H_
#define PAIRWISESALIENCEMASKGENERATOR_H_

#include "SalienceMaskGeneratorInterface.h"

class PairwiseSalienceMaskGenerator : public SalienceMaskGeneratorInterface
{
public:
	PairwiseSalienceMaskGenerator(int indexProximityThreshold = 1);
	~PairwiseSalienceMaskGenerator();

	virtual void generateSalienceMask(
		const ImageDatasetInterface& reference,
		const ImageDatasetInterface& query,
		cv::Mat& outputMask) const;

	virtual void generateSalienceMask(
		const ImageDatasetInterface& reference,
		const ImageDatasetInterface& query,
		std::list<ImageFilterInterface*>& filters,
		cv::Mat& outputMask) const;

private:
	int indexProx;
};

#endif /* PAIRWISESALIENCEMASKGENERATOR_H_ */