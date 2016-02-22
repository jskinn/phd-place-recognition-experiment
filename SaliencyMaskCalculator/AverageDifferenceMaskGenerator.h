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
	AverageDifferenceMaskGenerator(std::string outputDebugDir);
	virtual ~AverageDifferenceMaskGenerator();

	virtual ImageMatcherInterface* generateSalienceMask(
		const ImageDatasetInterface& reference,
		const ImageDatasetInterface& query) const;

private:
	std::string outputDebugDir;
};

#endif /* AVERAGEDIFFERENCEMASKGENERATOR_H_ */
