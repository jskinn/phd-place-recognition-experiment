/*
* MultipleDatasetComparisonExperiment.cpp
*
*  Created on: 19 Feb 2016
*      Author: john
*/

#include "stdafx.h"
#include "MultipleDatasetComparisonExperiment.h"

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
#include "WeightingSalienceMaskGenerator.h"
#include "SumOfAbsoluteDifferencesMatcher.h"

MultipleDatasetComparisonExperiment::MultipleDatasetComparisonExperiment()
{
}


MultipleDatasetComparisonExperiment::~MultipleDatasetComparisonExperiment()
{
}

void MultipleDatasetComparisonExperiment::runExperiment(std::string outputDir)
{
	// Set up the image filters
	DownsampleFilter dsf(64, 64);	// Dataset images are in a 1:1 ratio.
	GreyscaleFilter gf;
	std::list<ImageFilterInterface*> filters;
	filters.push_back(&dsf);
	filters.push_back(&gf);

	// Set up the training image datasets
	std::unique_ptr<CachedDataset> highVariationDataset = loadLeftAndRightVariableOrientationDataset(filters);
	std::cout << "Loaded reference dataset... (" << highVariationDataset->count() << " images)" << std::endl;

	std::unique_ptr<CachedDataset> testMiddleOfTheRoadPath = loadMiddleOfTheRoadDataset(filters);
	std::cout << "Loaded middle of road dataset... (" << testMiddleOfTheRoadPath->count() << " images)" << std::endl;
	std::unique_ptr<CachedDataset> testLeftLaneMorningPath = loadLeftLaneMorningDataset(filters);
	std::cout << "Loaded left lane morning dataset... (" << testLeftLaneMorningPath->count() << " images)" << std::endl;
	std::unique_ptr<CachedDataset> testLeftLaneMiddayPath = loadLeftLaneMiddayDataset(filters);
	std::cout << "Loaded left lane midday dataset... (" << testLeftLaneMiddayPath->count() << " images)" << std::endl;
	std::unique_ptr<CachedDataset> testLeftLaneSunsetPath = loadLeftLaneSunsetDataset(filters);
	std::cout << "Loaded left lane sunset dataset... (" << testLeftLaneSunsetPath->count() << " images)" << std::endl;
	std::cout << "Datasets loaded" << std::endl;

	// Create a RapidJson document to write the results into.
	rapidjson::Document results(rapidjson::kObjectType);

	// Set some standard criteria for considering two images to have a 'similar' location
	SimilarityCriteria similarityCriteria(300.0);

	// Verify the number of similar images between each of the datasets and the reference dataset
	/*float avgSimilar = calculateAverageSimilarImages(*highVariationDataset, *highVariationDataset, similarityCriteria);
	std::cout << "Average similar images for the reference dataset: " << avgSimilar << std::endl;
	results.AddMember("reference-average-similar-count", avgSimilar, results.GetAllocator());*/

	float avgSimilar = calculateAverageSimilarImages(*highVariationDataset, *testMiddleOfTheRoadPath, similarityCriteria);
	std::cout << "Average similar images for the middle of the road dataset: " << avgSimilar << std::endl;
	results.AddMember("middle-of-road-average-similar-count", avgSimilar, results.GetAllocator());

	avgSimilar = calculateAverageSimilarImages(*highVariationDataset, *testLeftLaneMorningPath, similarityCriteria);
	std::cout << "Average similar images for the left lane morning dataset: " << avgSimilar << std::endl;
	results.AddMember("left-lane-morning-average-similar-count", avgSimilar, results.GetAllocator());

	avgSimilar = calculateAverageSimilarImages(*highVariationDataset, *testLeftLaneMiddayPath, similarityCriteria);
	std::cout << "Average similar images for the left lane midday dataset: " << avgSimilar << std::endl;
	results.AddMember("left-lane-midday-average-similar-count", avgSimilar, results.GetAllocator());

	avgSimilar = calculateAverageSimilarImages(*highVariationDataset, *testLeftLaneSunsetPath, similarityCriteria);
	std::cout << "Average similar images for the left lane sunset dataset: " << avgSimilar << std::endl;
	results.AddMember("left-lane-sunset-average-similar-count", avgSimilar, results.GetAllocator());

	// Set up the place recognition object, salience mask generator, and output image
	PlaceRecognition placerecog;
	PairwiseSalienceMaskGenerator maskGen(similarityCriteria, 0.1f, outputDir);
	SumOfAbsoluteDifferencesMatcher sadMatcher;
	ImageMatcherInterface* salienceMask;
	cv::Mat diagonalMatrix;
	float performance;

	// Generate a salience mask based on the reference dataset. This will take forever.
	/*salienceMask = maskGen.generateSalienceMask(*highVariationDataset, *highVariationDataset);
	std::cout << "Generated salience mask" << std::endl;*/

	// For sanity check, verify on the same dataset used to generate the mask
	// TODO: Cut this because it takes too long.
	/*performance = placerecog.generateDiagonalMatrix(*highVariationDataset, *highVariationDataset, sadMatcher, similarityCriteria, diagonalMatrix);
	writeFloatImage(outputDir + "\\diagonal matrix reference without mask.png", diagonalMatrix);
	std::cout << "Matching accuracy for reference dataset without mask: " << (performance * 100) << "%" << std::endl;
	results.AddMember("reference-without-mask", performance, results.GetAllocator());

	performance = placerecog.generateDiagonalMatrix(*highVariationDataset, *highVariationDataset, *salienceMask, similarityCriteria, diagonalMatrix);
	writeFloatImage(outputDir + "\\diagonal matrix reference with mask.png", diagonalMatrix);
	std::cout << "Matching accuracy for reference dataset with mask: " << (performance * 100) << "%" << std::endl;
	results.AddMember("reference-with-mask", performance, results.GetAllocator());*/

	// Test the performance on a straight path with the salience mask
	performance = placerecog.generateDiagonalMatrix(*highVariationDataset, *testMiddleOfTheRoadPath, sadMatcher, similarityCriteria, diagonalMatrix);
	writeFloatImage(outputDir + "\\diagonal matrix middle of the road without mask.png", diagonalMatrix);
	std::cout << "Matching accuracy for middle of the road without mask: " << (performance * 100) << "%" << std::endl;
	results.AddMember("middle-of-the-road-without-mask", performance, results.GetAllocator());

	/*performance = placerecog.generateDiagonalMatrix(*highVariationDataset, *testMiddleOfTheRoadPath, *salienceMask, similarityCriteria, diagonalMatrix);
	writeFloatImage(outputDir + "\\diagonal matrix middle of the road with mask.png", diagonalMatrix);
	std::cout << "Matching accuracy for middle of the road with mask: " << (performance * 100) << "%" << std::endl;
	results.AddMember("middle-of-the-road-with-mask", performance, results.GetAllocator());*/

	// Test the performance on a straight path with the salience mask
	performance = placerecog.generateDiagonalMatrix(*highVariationDataset, *testLeftLaneMorningPath, sadMatcher, similarityCriteria, diagonalMatrix);
	writeFloatImage(outputDir + "\\diagonal matrix left lane morning without mask.png", diagonalMatrix);
	std::cout << "Matching accuracy for left lane morning without mask: " << (performance * 100) << "%" << std::endl;
	results.AddMember("left-lane-morning-without-mask", performance, results.GetAllocator());

	/*performance = placerecog.generateDiagonalMatrix(*highVariationDataset, *testLeftLaneMorningPath, *salienceMask, similarityCriteria, diagonalMatrix);
	writeFloatImage(outputDir + "\\diagonal matrix left lane morning with mask.png", diagonalMatrix);
	std::cout << "Matching accuracy for left lane morning with mask: " << (performance * 100) << "%" << std::endl;
	results.AddMember("left-lane-morning-with-mask", performance, results.GetAllocator());*/

	// Test the performance on a straight path with the salience mask
	performance = placerecog.generateDiagonalMatrix(*highVariationDataset, *testLeftLaneMiddayPath, sadMatcher, similarityCriteria, diagonalMatrix);
	writeFloatImage(outputDir + "\\diagonal matrix left lane midday without mask.png", diagonalMatrix);
	std::cout << "Matching accuracy for left lane midday without mask: " << (performance * 100) << "%" << std::endl;
	results.AddMember("left-lane-midday-without-mask", performance, results.GetAllocator());

	/*performance = placerecog.generateDiagonalMatrix(*highVariationDataset, *testLeftLaneMiddayPath, *salienceMask, similarityCriteria, diagonalMatrix);
	writeFloatImage(outputDir + "\\diagonal matrix left lane midday with mask.png", diagonalMatrix);
	std::cout << "Matching accuracy for left lane midday with mask: " << (performance * 100) << "%" << std::endl;
	results.AddMember("left-lane-midday-with-mask", performance, results.GetAllocator());*/

	// Test the performance on a straight path with the salience mask
	performance = placerecog.generateDiagonalMatrix(*highVariationDataset, *testLeftLaneSunsetPath, sadMatcher, similarityCriteria, diagonalMatrix);
	writeFloatImage(outputDir + "\\diagonal matrix left lane sunset without mask.png", diagonalMatrix);
	std::cout << "Matching accuracy for left lane sunset without mask: " << (performance * 100) << "%" << std::endl;
	results.AddMember("left-lane-sunset-without-mask", performance, results.GetAllocator());

	/*performance = placerecog.generateDiagonalMatrix(*highVariationDataset, *testLeftLaneSunsetPath, *salienceMask, similarityCriteria, diagonalMatrix);
	writeFloatImage(outputDir + "\\diagonal matrix left lane sunset with mask.png", diagonalMatrix);
	std::cout << "Matching accuracy for left lane sunset with mask: " << (performance * 100) << "%" << std::endl;
	results.AddMember("left-lane-sunset-with-mask", performance, results.GetAllocator());*/

	// Serialze the json output.
	rapidjson::StringBuffer jsonBuffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> jsonWriter(jsonBuffer);
	results.Accept(jsonWriter);

	// Write the json file to a string
	std::ofstream output(outputDir + "\\results.txt");
	output << jsonBuffer.GetString();
	output.close();

	// Clean up
	//delete salienceMask;
}

/**
 * Load a dataset with a variety offsets and camera angles.
 * Dataset size is 13475 images, so using it will take a long time.
 */
std::unique_ptr<CachedDataset> MultipleDatasetComparisonExperiment::loadLeftAndRightVariableOrientationDataset(const std::list<ImageFilterInterface*>& filters)
{
	std::list<ImageLoaderInterface*> loaders;

	ImageWithGroundTruthLoader imageLoader1("C:\\LocalUser\\Documents\\Renders\\city dataset 2016-02-18\\LeftAndRightVariableOrientationDataset\\Image_", ".png", 13475, 0, 1, 0, filters);
	loaders.push_back(&imageLoader1);

	return std::make_unique<CachedDataset>(loaders);
}

/**
 * Load a dataset that passes down the middle of the road in a single pass
 * Dataset size is 612 images
 */
std::unique_ptr<CachedDataset> MultipleDatasetComparisonExperiment::loadMiddleOfTheRoadDataset(const std::list<ImageFilterInterface*>& filters)
{
	std::list<ImageLoaderInterface*> loaders;

	ImageWithGroundTruthLoader imageLoader1("C:\\LocalUser\\Documents\\Renders\\city dataset 2016-02-18\\MiddleOfTheRoadDataset\\Image_", ".png", 612, 0, 1, 0, filters);
	loaders.push_back(&imageLoader1);

	return std::make_unique<CachedDataset>(loaders);
}

/**
 * Load a dataset that travels down the left lane in the morning
 * Dataset size is 609 images
 */
std::unique_ptr<CachedDataset> MultipleDatasetComparisonExperiment::loadLeftLaneMorningDataset(const std::list<ImageFilterInterface*>& filters)
{
	std::list<ImageLoaderInterface*> loaders;

	ImageWithGroundTruthLoader imageLoader1("C:\\LocalUser\\Documents\\Renders\\city dataset 2016-02-18\\Left-Lane\\DaytimeDataset\\Image_", ".png", 609, 0, 1, 0, filters);
	loaders.push_back(&imageLoader1);

	return std::make_unique<CachedDataset>(loaders);
}

/**
 * Load a dataset that travels down the left lane in the middle of the day
 * Dataset size is 609 images
 */
std::unique_ptr<CachedDataset> MultipleDatasetComparisonExperiment::loadLeftLaneMiddayDataset(const std::list<ImageFilterInterface*>& filters)
{
	std::list<ImageLoaderInterface*> loaders;

	ImageWithGroundTruthLoader imageLoader1("C:\\LocalUser\\Documents\\Renders\\city dataset 2016-02-18\\Left-Lane\\MiddayDataset\\Image_", ".png", 609, 0, 1, 0, filters);
	loaders.push_back(&imageLoader1);

	return std::make_unique<CachedDataset>(loaders);
}

/**
 * Load a dataset that travels down the left lane in the middle of the day
 * Dataset size is 609 images
 */
std::unique_ptr<CachedDataset> MultipleDatasetComparisonExperiment::loadLeftLaneSunsetDataset(const std::list<ImageFilterInterface*>& filters)
{
	std::list<ImageLoaderInterface*> loaders;

	ImageWithGroundTruthLoader imageLoader1("C:\\LocalUser\\Documents\\Renders\\city dataset 2016-02-18\\Left-Lane\\SunsetDataset\\Image_", ".png", 609, 0, 1, 0, filters);
	loaders.push_back(&imageLoader1);

	return std::make_unique<CachedDataset>(loaders);
}

/**
* Load a dataset that travels down the left lane in the middle of the day
* Dataset size is 13475 images
*/
std::unique_ptr<CachedDataset> MultipleDatasetComparisonExperiment::loadNightDataset(const std::list<ImageFilterInterface*>& filters)
{
	std::list<ImageLoaderInterface*> loaders;

	ImageWithGroundTruthLoader imageLoader1("C:\\LocalUser\\Documents\\Renders\\city dataset 2016-02-18\\MiddleOfTheRoadNightDatasetVariableOrientation\\Image_", ".png", 13475, 0, 1, 0, filters);
	loaders.push_back(&imageLoader1);

	return std::make_unique<CachedDataset>(loaders);
}

/**
 * A simple helper to write out float images from a range 0-1,
 * since im::write can only handle 8 bit images range 0-255
 */
void MultipleDatasetComparisonExperiment::writeFloatImage(std::string filename, const cv::Mat& floatImage)
{
	cv::Mat outputImage;
	double min = 0, max = 0;
	cv::minMaxLoc(floatImage, &min, &max);

	floatImage.convertTo(outputImage, CV_8UC1, 255.0 / max - min);
	cv::imwrite(filename, outputImage);
}

float MultipleDatasetComparisonExperiment::calculateAverageSimilarImages(const CachedDataset& reference, const CachedDataset& query, const SimilarityCriteria& similarityCriteria) const
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