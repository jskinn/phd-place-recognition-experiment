#pragma once
#include "SalienceMaskGeneratorInterface.h"
class AverageDifferenceMaskGenerator :
	public SalienceMaskGeneratorInterface
{
public:
	AverageDifferenceMaskGenerator();
	virtual ~AverageDifferenceMaskGenerator();

	virtual SalienceMask generateSalienceMask(
		const ImageDatasetInterface& reference,
		const ImageDatasetInterface& query) const;

	virtual SalienceMask generateSalienceMask(
		const ImageDatasetInterface& reference,
		const ImageDatasetInterface& query,
		std::list<ImageFilterInterface*>& filters) const;
};

