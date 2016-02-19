#include "stdafx.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <list>
#include <iostream>
#include <string>
#include <sstream>

#include "DownsampleFilter.h"
#include "GreyscaleFilter.h"
#include "LinearTraverseLoader.h"
#include "VPRiCELoader.h"
#include "CachedDataset.h"
#include "PlaceRecognition.h"
#include "ImageMatcherInterface.h"
#include "SumOfAbsoluteDifferencesMatcher.h"
#include "AverageDifferenceMaskGenerator.h"
#include "PairwiseSalienceMaskGenerator.h"
#include "WeightingSalienceMaskGenerator.h"
#include "SalienceMaskPerformanceExperiment.h"


SalienceMaskPerformanceExperiment::SalienceMaskPerformanceExperiment()
{
}


SalienceMaskPerformanceExperiment::~SalienceMaskPerformanceExperiment()
{
}

void SalienceMaskPerformanceExperiment::runExperiment(std::string outputDir)
{
	// Set up the image filters
	DownsampleFilter dsf(128, 72);	// Maintain 16:9 image ratio so we can compare images.
	GreyscaleFilter gf;
	std::list<ImageFilterInterface*> filters;
	filters.push_back(&dsf);
	filters.push_back(&gf);

	// Set up the training image datasets
	std::unique_ptr<CachedDataset> reference = loadReferenceDataset(filters);
	std::unique_ptr<CachedDataset> query = loadQueryDataset(filters);
	std::unique_ptr<CachedDataset> rwReference = loadReferenceRealWorldDataset(filters);
	std::unique_ptr<CachedDataset> rwQuery = loadQueryRealWorldDataset(filters);
	std::cout << "Datasets loaded" << std::endl;

	// Run the experiment
	// Set some standard criteria for considering two images to have a 'similar' location
	SimilarityCriteria similarityCriteria(300.0);

	// Set up the place recognition object, salience mask generator, and output image
	PlaceRecognition placerecog;
	WeightingSalienceMaskGenerator maskGen(similarityCriteria, 0.1f);
	PairwiseSalienceMaskGenerator alternateMaskGen(similarityCriteria);
	SumOfAbsoluteDifferencesMatcher sadMatcher;
	cv::Mat diagonalMatrix;
	ImageMatcherInterface* salienceMask;
	ImageMatcherInterface* alternateSalienceMask;
	float performanceWithoutMask = 0;
	float performanceWithMask = 0;

	// generate an initial diagonal matrix without a salience mask
	performanceWithoutMask = placerecog.generateDiagonalMatrix(*reference, *query, sadMatcher, similarityCriteria, diagonalMatrix);
	writeFloatImage(outputDir + "\\diagonal matrix no mask.png", diagonalMatrix);
	std::cout << "Created base diagonal matrix" << std::endl;
	std::cout << "Matching accuracy without salience mask: " << (performanceWithoutMask * 100) << "%" << std::endl;

	// Generate a salience mask
	salienceMask = maskGen.generateSalienceMask(*reference, *query);
	//writeFloatImage(outputDir + "\\salience mask.png", salienceMaskImage);
	std::cout << "Generated salience mask" << std::endl;

	// Generate a final diagonal matrix using the salience mask.
	performanceWithMask = placerecog.generateDiagonalMatrix(*reference, *query, *salienceMask, similarityCriteria, diagonalMatrix);
	writeFloatImage(outputDir + "\\diagonal matrix with logical mask.png", diagonalMatrix);
	std::cout << "Generated masked diagonal matrix" << std::endl;
	std::cout << "Matching accuracy with logical salience mask: " << (performanceWithMask * 100) << "%" << std::endl;

	// Generate another salience mask
	alternateSalienceMask = alternateMaskGen.generateSalienceMask(*reference, *query);
	//writeFloatImage(outputDir + "\\salience mask.png", salienceMaskImage);
	std::cout << "Generated salience mask" << std::endl;

	// Generate a final diagonal matrix using the salience mask.
	performanceWithMask = placerecog.generateDiagonalMatrix(*reference, *query, *alternateSalienceMask, similarityCriteria, diagonalMatrix);
	writeFloatImage(outputDir + "\\diagonal matrix with pairwise mask.png", diagonalMatrix);
	std::cout << "Generated masked diagonal matrix" << std::endl;
	std::cout << "Matching accuracy with threshold salience mask: " << (performanceWithMask * 100) << "%" << std::endl;

	// Test on real-world data
	std::cout << "Testing on real world data..." << std::endl;
	SimilarityCriteria rwSimilarityCriteria(0.1);	// Tiny radius since adjacent images don't help. There are multiple merged datasets, and I haven't worked out how to separated them.
	performanceWithoutMask = placerecog.generateDiagonalMatrix(*rwReference, *rwQuery, sadMatcher, rwSimilarityCriteria, diagonalMatrix);
	writeFloatImage(outputDir + "\\real world diagonal matrix without mask.png", diagonalMatrix);
	performanceWithMask = placerecog.generateDiagonalMatrix(*rwReference, *rwQuery, *salienceMask, rwSimilarityCriteria, diagonalMatrix);	//TODO: Change to a matcher using the salience mask.
	writeFloatImage(outputDir + "\\real world diagonal matrix with logical mask.png", diagonalMatrix);
	std::cout << "Real-world matching without mask " << (performanceWithoutMask * 100) << "% and with logical mask " << (performanceWithMask * 100) << "%" << std::endl;

	std::system("pause");

	// Clean up
	delete salienceMask;
	delete alternateSalienceMask;
}

/**
* A simple helper to write out float images from a range 0-1,
* since im::write can only handle 8 bit images range 0-255
*/
void SalienceMaskPerformanceExperiment::writeFloatImage(std::string filename, cv::Mat& floatImage)
{
	cv::Mat outputImage;
	floatImage.convertTo(outputImage, CV_8UC1, 255.0);
	cv::imwrite(filename, outputImage);
}

/**
* Construct the reference dataset.
* Can load images from a number of locations.
*/
std::unique_ptr<CachedDataset> SalienceMaskPerformanceExperiment::loadReferenceDataset(const std::list<ImageFilterInterface*>& filters)
{
	std::list<ImageLoaderInterface*> loaders;

	//LinearTraverseLoader imageLoader(cv::Vec3d(14200.0, -4000.0, 0.0), cv::Vec3d(14200.0, 13000, 0.0), cv::Vec3d(0.0, 0.0, 90.0), "C:\\LocalUser\\Documents\\Renders\\city dataset 2016-01-21\\x 14200\\MovieCapture_640x360_1.00 ", ".png", 600, 2, 1, 10, filters);
	//LinearTraverseLoader imageLoader(cv::Vec3d(14200.0, -4000.0, 0.0), cv::Vec3d(14200.0, -1166.6666666, 0.0), cv::Vec3d(0.0, 0.0, 90.0), "C:\\LocalUser\\Documents\\Renders\\city dataset 2016-01-21\\x 14200\\MovieCapture_640x360_1.00 ", ".png", 100, 2, 1, 10, filters);
	LinearTraverseLoader imageLoader1(cv::Vec3d(14200.0, -4000.0, 0.0), cv::Vec3d(14200.0, 13000, 0.0), cv::Vec3d(0.0, 0.0, 90.0), "C:\\LocalUser\\Documents\\Renders\\city dataset 2016-01-21\\x 14200\\MovieCapture_640x360_1.00 ", ".png", 600, 2, 1, 10, filters);
	loaders.push_back(&imageLoader1);

	LinearTraverseLoader imageLoader2(cv::Vec3d(15200.0, -4000.0, 0.0), cv::Vec3d(15200.0, 13000, 0.0), cv::Vec3d(0.0, 0.0, 90.0), "C:\\LocalUser\\Documents\\Renders\\city dataset 2016-01-21\\x 15200\\MovieCapture_640x360_1.00 ", ".png", 600, 2, 1, 10, filters);
	loaders.push_back(&imageLoader2);

	return std::make_unique<CachedDataset>(loaders);
}

/**
* Construct the query dataset.
*/
std::unique_ptr<CachedDataset> SalienceMaskPerformanceExperiment::loadQueryDataset(const std::list<ImageFilterInterface*>& filters)
{
	std::list<ImageLoaderInterface*> loaders;

	//LinearTraverseLoader imageLoader(cv::Vec3d(14400.0, -4000.0, 0.0), cv::Vec3d(14400.0, 13000.0, 0.0), cv::Vec3d(0.0, 0.0, 90.0), "C:\\LocalUser\\Documents\\Renders\\city dataset 2016-01-21\\x 14400\\MovieCapture_640x360_1.00 ", ".png", 600, 2, 1, 10, filters);
	LinearTraverseLoader imageLoader1(cv::Vec3d(14400.0, -4000.0, 0.0), cv::Vec3d(14400.0, 13000.0, 0.0), cv::Vec3d(0.0, 0.0, 90.0), "C:\\LocalUser\\Documents\\Renders\\city dataset 2016-01-21\\x 14400\\MovieCapture_640x360_1.00 ", ".png", 600, 2, 1, 10, filters);
	loaders.push_back(&imageLoader1);

	LinearTraverseLoader imageLoader2(cv::Vec3d(15000.0, -4000.0, 0.0), cv::Vec3d(15000.0, 13000.0, 0.0), cv::Vec3d(0.0, 0.0, 90.0), "C:\\LocalUser\\Documents\\Renders\\city dataset 2016-01-21\\x 15000\\MovieCapture_640x360_1.00 ", ".png", 600, 2, 1, 10, filters);
	loaders.push_back(&imageLoader2);

	return std::make_unique<CachedDataset>(loaders);
}

std::unique_ptr<CachedDataset> SalienceMaskPerformanceExperiment::loadReferenceRealWorldDataset(const std::list<ImageFilterInterface*>& filters)
{
	std::list<ImageLoaderInterface*> loaders;

	VPRiCELoader imageLoader1("C:\\LocalUser\\Documents\\Renders\\VPRiCE-dataset\\live\\", 10, 0, 2, filters);
	loaders.push_back(&imageLoader1);

	return std::make_unique<CachedDataset>(loaders);
}

std::unique_ptr<CachedDataset> SalienceMaskPerformanceExperiment::loadQueryRealWorldDataset(const std::list<ImageFilterInterface*>& filters)
{
	std::list<ImageLoaderInterface*> loaders;

	//VPRiCELoader imageLoader1("C:\\LocalUser\\Documents\\Renders\\VPRiCE-dataset\\memory\\", 100, 0, 1, filters);
	VPRiCELoader imageLoader1("C:\\LocalUser\\Documents\\Renders\\VPRiCE-dataset\\live\\", 10, 1, 2, filters);
	loaders.push_back(&imageLoader1);

	return std::make_unique<CachedDataset>(loaders);
}
