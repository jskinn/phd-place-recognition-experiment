/*
* MultipleDatasetComparisonExperiment.h
*
*  Created on: 19 Feb 2016
*      Author: john
*/

#ifndef SALIENCEMASKPERFORMANCEEXPERIMENT_H_
#define SALIENCEMASKPERFORMANCEEXPERIMENT_H_

#include <string>
#include <list>
#include <memory>
#include <opencv2/core/core.hpp>
#include "CachedDataset.h"
#include "ImageFilterInterface.h"

class MultipleDatasetComparisonExperiment
{
public:
	MultipleDatasetComparisonExperiment();
	~MultipleDatasetComparisonExperiment();

	void runExperiment(std::string outputDir);

private:
	void writeFloatImage(std::string filename, cv::Mat& floatImage);

	std::unique_ptr<CachedDataset> loadLeftAndRightVariableOrientationDataset(const std::list<ImageFilterInterface*>& filters);
	std::unique_ptr<CachedDataset> loadMiddleOfTheRoadDataset(const std::list<ImageFilterInterface*>& filters);
	std::unique_ptr<CachedDataset> loadLeftLaneMorningDataset(const std::list<ImageFilterInterface*>& filters);
	std::unique_ptr<CachedDataset> loadLeftLaneMiddayDataset(const std::list<ImageFilterInterface*>& filters);
	std::unique_ptr<CachedDataset> loadLeftLaneSunsetDataset(const std::list<ImageFilterInterface*>& filters);
};

#endif /* SALIENCEMASKPERFORMANCEEXPERIMENT_H_ */
