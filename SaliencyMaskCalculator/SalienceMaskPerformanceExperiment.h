/*
* SalienceMaskPerformanceExperiment.h
*
*  Created on: 19 Feb 2016
*      Author: john
*/

#ifndef SALIENCEMASKPERFORMANCEEXPERIMENT_H_
#define SALIENCEMASKPERFORMANCEEXPERIMENT_H_

#include <memory>
#include <opencv2/core/core.hpp>
#include "CachedDataset.h"

class SalienceMaskPerformanceExperiment
{
public:
	SalienceMaskPerformanceExperiment();
	~SalienceMaskPerformanceExperiment();

	void runExperiment(std::string outputDir);

private:
	void writeFloatImage(std::string filename, cv::Mat& floatImage);

	std::unique_ptr<CachedDataset> loadReferenceDataset(const std::list<ImageFilterInterface*>& filters);
	std::unique_ptr<CachedDataset> loadQueryDataset(const std::list<ImageFilterInterface*>& filters);
	std::unique_ptr<CachedDataset> loadReferenceRealWorldDataset(const std::list<ImageFilterInterface*>& filters);
	std::unique_ptr<CachedDataset> loadQueryRealWorldDataset(const std::list<ImageFilterInterface*>& filters);
};

#endif /* SALIENCEMASKPERFORMANCEEXPERIMENT_H_ */
