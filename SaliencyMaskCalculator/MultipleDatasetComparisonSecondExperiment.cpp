/*
* MultipleDatasetComparisonSecondExperiment.cpp
*
*  Created on: 19 Feb 2016
*      Author: john
*/

#include "stdafx.h"
#include "MultipleDatasetComparisonSecondExperiment.h"

#include <memory>
#include <iostream>
#include <fstream>
#include <opencv2/highgui/highgui.hpp>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>
#include "DownsampleFilter.h"
#include "GreyscaleFilter.h"
#include "ImageWithGroundTruthLoader.h"
#include "PlaceRecognition.h"
#include "WeightingSalienceMaskGenerator.h"
#include "PairwiseSalienceMaskGenerator.h"
#include "ImageMatcherInterface.h"
#include "SumOfAbsoluteDifferencesMatcher.h"

const int MultipleDatasetComparisonSecondExperiment::TIME_OF_DAY_DAWN = 0;
const int MultipleDatasetComparisonSecondExperiment::TIME_OF_DAY_MORNING = 1;
const int MultipleDatasetComparisonSecondExperiment::TIME_OF_DAY_NOON = 2;
const int MultipleDatasetComparisonSecondExperiment::TIME_OF_DAY_AFTERNOON = 3;
const int MultipleDatasetComparisonSecondExperiment::TIME_OF_DAY_SUNSET = 4;

const int MultipleDatasetComparisonSecondExperiment::PASS_FIRST = 0;
const int MultipleDatasetComparisonSecondExperiment::PASS_LAST = 18;
const int MultipleDatasetComparisonSecondExperiment::PASS_BASELINE = 0;
const int MultipleDatasetComparisonSecondExperiment::PASS_DOWN_15 = 1;
const int MultipleDatasetComparisonSecondExperiment::PASS_DOWN_30 = 2;
const int MultipleDatasetComparisonSecondExperiment::PASS_LEFT_15 = 3;
const int MultipleDatasetComparisonSecondExperiment::PASS_LEFT_30 = 4;
const int MultipleDatasetComparisonSecondExperiment::PASS_LEFT_OFFSET_250 = 5;
const int MultipleDatasetComparisonSecondExperiment::PASS_LEFT_OFFSET_250_DOWN_15 = 6;
const int MultipleDatasetComparisonSecondExperiment::PASS_LEFT_OFFSET_250_UP_15 = 7;
const int MultipleDatasetComparisonSecondExperiment::PASS_LEFT_OFFSET_500 = 8;
const int MultipleDatasetComparisonSecondExperiment::PASS_LEFT_OFFSET_900 = 9;
const int MultipleDatasetComparisonSecondExperiment::PASS_RIGHT_15 = 10;
const int MultipleDatasetComparisonSecondExperiment::PASS_RIGHT_30 = 11;
const int MultipleDatasetComparisonSecondExperiment::PASS_RIGHT_OFFSET_250 = 12;
const int MultipleDatasetComparisonSecondExperiment::PASS_RIGHT_OFFSET_250_DOWN_15 = 13;
const int MultipleDatasetComparisonSecondExperiment::PASS_RIGHT_OFFSET_250_UP_15 = 14;
const int MultipleDatasetComparisonSecondExperiment::PASS_RIGHT_OFFSET_500 = 15;
const int MultipleDatasetComparisonSecondExperiment::PASS_RIGHT_OFFSET_900 = 16;
const int MultipleDatasetComparisonSecondExperiment::PASS_UP_15 = 17;
const int MultipleDatasetComparisonSecondExperiment::PASS_UP_30 = 18;

MultipleDatasetComparisonSecondExperiment::MultipleDatasetComparisonSecondExperiment()
{
}


MultipleDatasetComparisonSecondExperiment::~MultipleDatasetComparisonSecondExperiment()
{
}

void MultipleDatasetComparisonSecondExperiment::runExperiment(std::string outputDir)
{
	// Set up the image filters
	DownsampleFilter dsf(64, 64);	// Dataset images are in a 1:1 ratio.
	GreyscaleFilter gf;
	std::list<ImageFilterInterface*> filters;
	filters.push_back(&dsf);
	filters.push_back(&gf);

	// Load the reference dataset
	std::unique_ptr<CachedDataset> referenceDataset = loadDataset(
		MultipleDatasetComparisonSecondExperiment::TIME_OF_DAY_NOON,
		MultipleDatasetComparisonSecondExperiment::PASS_BASELINE,
		filters);
	std::cout << "Loaded reference dataset... (" << referenceDataset->count() << " images)" << std::endl;

	// Set up the place recognition object, salience mask generator, and output image
	PlaceRecognition placerecog;
	SumOfAbsoluteDifferencesMatcher sadMatcher;
	cv::Mat diagonalMatrix;
	float performance;

	// Create a RapidJson document to write the results into.
	rapidjson::Document results(rapidjson::kObjectType);

	// Set some standard criteria for considering two images to have a 'similar' location
	SimilarityCriteria similarityCriteria(300.0);

	// For each test time of day, for each pass through, measure the performance
	// This will match the reference set against itself
	for (int timeOfDay = MultipleDatasetComparisonSecondExperiment::TIME_OF_DAY_DAWN;
	timeOfDay <= MultipleDatasetComparisonSecondExperiment::TIME_OF_DAY_SUNSET; ++timeOfDay)
	{
		std::string timeOfDayString = getTimeOfDayString(timeOfDay);
		rapidjson::Value nestedResults(rapidjson::kObjectType);

		for (int pass = MultipleDatasetComparisonSecondExperiment::PASS_FIRST;
		pass <= MultipleDatasetComparisonSecondExperiment::PASS_LAST; ++pass)
		{
			std::string passString = getPassString(pass);
			std::unique_ptr<CachedDataset> queryDataset = loadDataset(timeOfDay, pass, filters);
			std::cout << "Loaded test dataset \"" << timeOfDayString << "\" subset \"" << passString << "\" (" << queryDataset->count() << " images)" << std::endl;

			performance = placerecog.generateDiagonalMatrix(*referenceDataset, *queryDataset, sadMatcher, similarityCriteria, diagonalMatrix);
			writeFloatImage(outputDir + "\\diagonal matrix " + timeOfDayString + " " + passString + ".png", diagonalMatrix);
			std::cout << "Matching accuracy for " << timeOfDayString << " " << passString << ": " << (performance * 100) << "%" << std::endl;

			nestedResults.AddMember(
				rapidjson::Value(passString.c_str(), results.GetAllocator()).Move(),
				performance,
				results.GetAllocator());
		}

		results.AddMember(
			rapidjson::Value(timeOfDayString.c_str(), results.GetAllocator()).Move(),
			nestedResults,
			results.GetAllocator());
	}

	// Serialze the json output.
	rapidjson::StringBuffer jsonBuffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> jsonWriter(jsonBuffer);
	results.Accept(jsonWriter);

	// Write the json file to a string
	std::ofstream output(outputDir + "\\multiple comparison results.txt");
	output << jsonBuffer.GetString();
	output.close();
}

/**
* Load a reference dataset. It is the basline, noon down the middle, so we can see how match performance falls of with offset and time of day
*/
std::unique_ptr<CachedDataset> MultipleDatasetComparisonSecondExperiment::loadDataset(int timeOfDay, int pass, const std::list<ImageFilterInterface*>& filters) const
{
	std::list<ImageLoaderInterface*> loaders;

	ImageWithGroundTruthLoader imageLoader1("C:\\LocalUser\\Documents\\Renders\\city dataset 2016-02-22\\" + getTimeOfDayString(timeOfDay) +"\\" + getPassString(pass) + "\\Image_", ".png", 245, 0, 1, 0, filters);
	loaders.push_back(&imageLoader1);

	return std::make_unique<CachedDataset>(loaders);
}

std::string MultipleDatasetComparisonSecondExperiment::getTimeOfDayString(int timeOfDay) const
{
	switch (timeOfDay) {
	case MultipleDatasetComparisonSecondExperiment::TIME_OF_DAY_DAWN:
		return "Dawn sun angle -10";
		break;
	case MultipleDatasetComparisonSecondExperiment::TIME_OF_DAY_MORNING:
		return "Morning sun angle -45";
		break;
	case MultipleDatasetComparisonSecondExperiment::TIME_OF_DAY_NOON:
	default:
		return "Noon sun angle -90";
		break;
	case MultipleDatasetComparisonSecondExperiment::TIME_OF_DAY_AFTERNOON:
		return "Afternoon sun angle -135";
		break;
	case MultipleDatasetComparisonSecondExperiment::TIME_OF_DAY_SUNSET:
		return "Sunset sun angle -170";
		break;
	}
}

std::string MultipleDatasetComparisonSecondExperiment::getPassString(int pass) const
{
	switch (pass) {
	case MultipleDatasetComparisonSecondExperiment::PASS_BASELINE:
	default:
		return "BaselineDataset";
		break;
	case MultipleDatasetComparisonSecondExperiment::PASS_DOWN_15:
		return "Down15Dataset";
		break;
	case MultipleDatasetComparisonSecondExperiment::PASS_DOWN_30:
		return "Down30Dataset";
		break;
	case MultipleDatasetComparisonSecondExperiment::PASS_LEFT_15:
		return "Left15Dataset";
		break;
	case MultipleDatasetComparisonSecondExperiment::PASS_LEFT_30:
		return "Left30Dataset";
		break;
	case MultipleDatasetComparisonSecondExperiment::PASS_LEFT_OFFSET_250:
		return "LeftOffset250Dataset";
		break;
	case MultipleDatasetComparisonSecondExperiment::PASS_LEFT_OFFSET_250_DOWN_15:
		return "LeftOffset250Down15";
		break;
	case MultipleDatasetComparisonSecondExperiment::PASS_LEFT_OFFSET_250_UP_15:
		return "LeftOffset250Up15";
		break;
	case MultipleDatasetComparisonSecondExperiment::PASS_LEFT_OFFSET_500:
		return "LeftOffset500Dataset";
		break;
	case MultipleDatasetComparisonSecondExperiment::PASS_LEFT_OFFSET_900:
		return "LeftOffset900Dataset";
		break;
	case MultipleDatasetComparisonSecondExperiment::PASS_RIGHT_15:
		return "Right15Dataset";
		break;
	case MultipleDatasetComparisonSecondExperiment::PASS_RIGHT_30:
		return "Right30Dataset";
		break;
	case MultipleDatasetComparisonSecondExperiment::PASS_RIGHT_OFFSET_250:
		return "RightOffset250Dataset";
		break;
	case MultipleDatasetComparisonSecondExperiment::PASS_RIGHT_OFFSET_250_DOWN_15:
		return "RightOffset250Down15";
		break;
	case MultipleDatasetComparisonSecondExperiment::PASS_RIGHT_OFFSET_250_UP_15:
		return "RightOffset250Up15";
		break;
	case MultipleDatasetComparisonSecondExperiment::PASS_RIGHT_OFFSET_500:
		return "RightOffset500Dataset";
		break;
	case MultipleDatasetComparisonSecondExperiment::PASS_RIGHT_OFFSET_900:
		return "RightOffset900Dataset";
		break;
	case MultipleDatasetComparisonSecondExperiment::PASS_UP_15:
		return "Up15Dataset";
		break;
	case MultipleDatasetComparisonSecondExperiment::PASS_UP_30:
		return "Up30Dataset";
		break;
	}
}

/**
* A simple helper to write out float images from a range 0-1,
* since im::write can only handle 8 bit images range 0-255
*/
void MultipleDatasetComparisonSecondExperiment::writeFloatImage(std::string filename, const cv::Mat& floatImage)
{
	cv::Mat outputImage;
	double min = 0, max = 0;
	cv::minMaxLoc(floatImage, &min, &max);

	floatImage.convertTo(outputImage, CV_8UC1, 255.0 / max - min);
	cv::imwrite(filename, outputImage);
}

float MultipleDatasetComparisonSecondExperiment::calculateAverageSimilarImages(const CachedDataset& reference, const CachedDataset& query, const SimilarityCriteria& similarityCriteria) const
{
	float meanSimilar = 0;
	for (int i = 0; i < query.count(); ++i) {
		for (int j = 0; j < reference.count(); ++j) {
			if (similarityCriteria.isImageSimilar(query.get(i), reference.get(j))) {
				meanSimilar++;
			}
		}
	}

	return meanSimilar / query.count();
}
