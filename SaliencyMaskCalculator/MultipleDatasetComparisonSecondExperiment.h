/*
* MultipleDatasetComparisonSecondExperiment.h
*
*  Created on: 22 Feb 2016
*      Author: john
*/

#ifndef MULTIPLEDATASETCOMPARISONSECONDEXPERIMENT_H_
#define MULTIPLEDATASETCOMPARISONSECONDEXPERIMENT_H_

#include <string>
#include <list>
#include <memory>
#include <opencv2/core/core.hpp>
#include "CachedDataset.h"
#include "SimilarityCriteria.h"
#include "ImageFilterInterface.h"

class MultipleDatasetComparisonSecondExperiment
{
public:
	MultipleDatasetComparisonSecondExperiment();
	~MultipleDatasetComparisonSecondExperiment();

	void runExperiment(std::string outputDir);

private:
	float calculateAverageSimilarImages(const CachedDataset& reference, const CachedDataset& query, const SimilarityCriteria& similarityCriteria) const;

	void writeFloatImage(std::string filename, const cv::Mat& floatImage);

	std::unique_ptr<CachedDataset> loadDataset(int timeOfDay, int pass, const std::list<ImageFilterInterface*>& filters) const;

	std::string getTimeOfDayString(int timeOfDay) const;
	std::string getPassString(int pass) const;

	static const int TIME_OF_DAY_DAWN;
	static const int TIME_OF_DAY_MORNING;
	static const int TIME_OF_DAY_NOON;
	static const int TIME_OF_DAY_AFTERNOON;
	static const int TIME_OF_DAY_SUNSET;

	static const int PASS_FIRST;
	static const int PASS_LAST;
	static const int PASS_BASELINE;
	static const int PASS_DOWN_15;
	static const int PASS_DOWN_30;
	static const int PASS_LEFT_15;
	static const int PASS_LEFT_30;
	static const int PASS_LEFT_OFFSET_250;
	static const int PASS_LEFT_OFFSET_250_DOWN_15;
	static const int PASS_LEFT_OFFSET_250_UP_15;
	static const int PASS_LEFT_OFFSET_500;
	static const int PASS_LEFT_OFFSET_900;
	static const int PASS_RIGHT_15;
	static const int PASS_RIGHT_30;
	static const int PASS_RIGHT_OFFSET_250;
	static const int PASS_RIGHT_OFFSET_250_DOWN_15;
	static const int PASS_RIGHT_OFFSET_250_UP_15;
	static const int PASS_RIGHT_OFFSET_500;
	static const int PASS_RIGHT_OFFSET_900;
	static const int PASS_UP_15;
	static const int PASS_UP_30;
};

#endif /* MULTIPLEDATASETCOMPARISONEXPERIMENT_H_ */