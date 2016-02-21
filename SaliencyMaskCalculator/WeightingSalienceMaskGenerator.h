/*
* WeightingSalienceMaskGenerator.h
*
*  Created on: 4 Feb 2016
*      Author: john
*/

#ifndef WEIGHTINGSALIENCEMASKGENERATOR_H_
#define WEIGHTINGSALIENCEMASKGENERATOR_H_

#include <string>
#include "SalienceMaskGeneratorInterface.h"
#include "SimilarityCriteria.h"

class WeightingSalienceMaskGenerator : public SalienceMaskGeneratorInterface
{
public:
	WeightingSalienceMaskGenerator(const SimilarityCriteria& similarityCriteria, float salienceFraction, std::string outputDebugDir);
	~WeightingSalienceMaskGenerator();

	virtual ImageMatcherInterface* generateSalienceMask(
		const ImageDatasetInterface& reference,
		const ImageDatasetInterface& query) const;

private:
	SimilarityCriteria criteria;
	float salienceFraction;

	std::string outputDebug;
};

#endif /* PAIRWISESALIENCEMASKGENERATOR_H_ */