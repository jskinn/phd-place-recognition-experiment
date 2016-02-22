/*
* PairwiseSalienceMaskGenerator.h
*
*  Created on: 29 Jan 2016
*      Author: john
*/

#ifndef PAIRWISESALIENCEMASKGENERATOR_H_
#define PAIRWISESALIENCEMASKGENERATOR_H_

#include "SalienceMaskGeneratorInterface.h"
#include "SimilarityCriteria.h"

class PairwiseSalienceMaskGenerator : public SalienceMaskGeneratorInterface
{
public:
	PairwiseSalienceMaskGenerator(const SimilarityCriteria& similarityCriteria, float salienceFraction, std::string outputDebugDir);
	~PairwiseSalienceMaskGenerator();

	virtual ImageMatcherInterface* generateSalienceMask(
		const ImageDatasetInterface& reference,
		const ImageDatasetInterface& query) const;

private:
	SimilarityCriteria criteria;
	float salienceFraction;
	std::string outputDebugDir;
};

#endif /* PAIRWISESALIENCEMASKGENERATOR_H_ */