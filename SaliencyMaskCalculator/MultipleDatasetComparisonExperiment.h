/*
* MultipleDatasetComparisonExperiment.h
*
*  Created on: 19 Feb 2016
*      Author: john
*/

#ifndef MULTIPLEDATASETCOMPARISONEXPERIMENT_H_
#define MULTIPLEDATASETCOMPARISONEXPERIMENT_H_

#include <string>
#include <list>
#include <memory>
#include <opencv2/core/core.hpp>
#include "CachedDataset.h"
#include "SimilarityCriteria.h"
#include "ImageFilterInterface.h"

class MultipleDatasetComparisonExperiment
{
public:
	MultipleDatasetComparisonExperiment();
	~MultipleDatasetComparisonExperiment();

	void runExperiment(std::string outputDir);

private:
	float calculateAverageSimilarImages(const CachedDataset& reference, const CachedDataset& query, const SimilarityCriteria& similarityCriteria) const;

	void writeFloatImage(std::string filename, const cv::Mat& floatImage);

	std::unique_ptr<CachedDataset> loadLeftAndRightVariableOrientationDataset(const std::list<ImageFilterInterface*>& filters);
	std::unique_ptr<CachedDataset> loadMiddleOfTheRoadDataset(const std::list<ImageFilterInterface*>& filters);
	std::unique_ptr<CachedDataset> loadLeftLaneMorningDataset(const std::list<ImageFilterInterface*>& filters);
	std::unique_ptr<CachedDataset> loadLeftLaneMiddayDataset(const std::list<ImageFilterInterface*>& filters);
	std::unique_ptr<CachedDataset> loadLeftLaneSunsetDataset(const std::list<ImageFilterInterface*>& filters);
	std::unique_ptr<CachedDataset> loadNightDataset(const std::list<ImageFilterInterface*>& filters);
};

#endif /* MULTIPLEDATASETCOMPARISONEXPERIMENT_H_ */
