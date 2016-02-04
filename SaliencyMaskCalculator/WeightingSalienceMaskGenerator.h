/*
* WeightingSalienceMaskGenerator.h
*
*  Created on: 4 Feb 2016
*      Author: john
*/

#ifndef WEIGHTINGSALIENCEMASKGENERATOR_H_
#define WEIGHTINGSALIENCEMASKGENERATOR_H_

#include "SalienceMaskGeneratorInterface.h"
#include "SimilarityCriteria.h"

class WeightingSalienceMaskGenerator : public SalienceMaskGeneratorInterface
{
public:
	WeightingSalienceMaskGenerator(const SimilarityCriteria& similarityCriteria);
	~WeightingSalienceMaskGenerator();

	virtual SalienceMaskInterface* generateSalienceMask(
		const ImageDatasetInterface& reference,
		const ImageDatasetInterface& query) const;

private:
	SimilarityCriteria criteria;
};

#endif /* PAIRWISESALIENCEMASKGENERATOR_H_ */