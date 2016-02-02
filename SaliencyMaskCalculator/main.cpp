/*
* SaliencyMaskCalculator.cpp
*
*  Created on: 25 Jan 2016
*      Author: john
*/

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
#include "CachedDataset.h"
#include "PlaceRecognition.h"
#include "AverageDifferenceMaskGenerator.h"
#include "PairwiseSalienceMaskGenerator.h"
#include "ThresholdSalienceMask.h"
#include "WeightingSalienceMask.h"

/**
 * A simple helper to write out float images from a range 0-1,
 * since im::write can only handle 8 bit images range 0-255
 */
void writeFloatImage(std::string filename, cv::Mat& floatImage)
{
	cv::Mat outputImage;
	floatImage.convertTo(outputImage, CV_8UC1, 255.0);
	cv::imwrite(filename, outputImage);
}

/**
 * Construct the reference dataset.
 * Can load images from a number of locations.
 */
CachedDataset* buildReferenceDataset(const std::list<ImageFilterInterface*>& filters)
{
	std::list<ImageLoaderInterface*> loaders;

	//LinearTraverseLoader imageLoader(cv::Vec3d(14200.0, -4000.0, 0.0), cv::Vec3d(14200.0, 13000, 0.0), cv::Vec3d(0.0, 0.0, 90.0), "C:\\LocalUser\\Documents\\Renders\\city dataset 2016-01-21\\x 14200\\MovieCapture_640x360_1.00 ", ".png", 600, 2, 1, 10, filters);
	//LinearTraverseLoader imageLoader(cv::Vec3d(14200.0, -4000.0, 0.0), cv::Vec3d(14200.0, -1166.6666666, 0.0), cv::Vec3d(0.0, 0.0, 90.0), "C:\\LocalUser\\Documents\\Renders\\city dataset 2016-01-21\\x 14200\\MovieCapture_640x360_1.00 ", ".png", 100, 2, 1, 10, filters);
	LinearTraverseLoader imageLoader(cv::Vec3d(14200.0, -4000.0, 0.0), cv::Vec3d(14200.0, 13000, 0.0), cv::Vec3d(0.0, 0.0, 90.0), "C:\\LocalUser\\Documents\\Renders\\city dataset 2016-01-21\\x 14200\\MovieCapture_640x360_1.00 ", ".png", 60, 2, 10, 10, filters);
	loaders.push_back(&imageLoader);

	return new CachedDataset(loaders);
}

/**
 * Construct the query dataset.
 */
CachedDataset* buildQueryDataset(const std::list<ImageFilterInterface*>& filters)
{
	std::list<ImageLoaderInterface*> loaders;

	//LinearTraverseLoader imageLoader(cv::Vec3d(14400.0, -4000.0, 0.0), cv::Vec3d(14400.0, 13000.0, 0.0), cv::Vec3d(0.0, 0.0, 90.0), "C:\\LocalUser\\Documents\\Renders\\city dataset 2016-01-21\\x 14400\\MovieCapture_640x360_1.00 ", ".png", 600, 2, 1, 10, filters);
	LinearTraverseLoader imageLoader(cv::Vec3d(14400.0, -4000.0, 0.0), cv::Vec3d(14400.0, 13000.0, 0.0), cv::Vec3d(0.0, 0.0, 90.0), "C:\\LocalUser\\Documents\\Renders\\city dataset 2016-01-21\\x 14400\\MovieCapture_640x360_1.00 ", ".png", 60, 2, 10, 10, filters);
	loaders.push_back(&imageLoader);

	return new CachedDataset(loaders);
}

/**
 * Run the experiment, calculating the vanilla performance, salience mask and masked performance.
 */
void runExperiment(ImageDatasetInterface& reference, ImageDatasetInterface& query)
{
	// Set up the place recognition object, salience mask generator, and output image
	PlaceRecognition placerecog;
	PairwiseSalienceMaskGenerator maskGen(SimilarityCriteria(400.0));
	cv::Mat diagonalMatrix, salienceMaskImage;

	// generate an initial diagonal matrix without a salience mask
	placerecog.generateDiagonalMatrix(reference, query, diagonalMatrix);
	writeFloatImage("C:\\LocalUser\\Documents\\Renders\\city dataset 2016-01-21\\diagonal matrix no mask.png", diagonalMatrix);
	std::cout << "Created base diagonal matrix" << std::endl;

	// Print the matching accuracy without the salience mask
	//float performanceWithoutMask = placerecog.measurePerformance(diagonalMatrix, 1);
	std::cout << "Matching accuracy without salience mask: (" <<
		placerecog.measurePerformance(diagonalMatrix, 0) << ", " <<
		placerecog.measurePerformance(diagonalMatrix, 1) << ", " <<
		placerecog.measurePerformance(diagonalMatrix, 3) << ", " <<
		placerecog.measurePerformance(diagonalMatrix, 5) << ", " <<
		placerecog.measurePerformance(diagonalMatrix, 7) << ", " <<
		placerecog.measurePerformance(diagonalMatrix, 9) << ")" << std::endl;

	/*cv::namedWindow("Display window", cv::WINDOW_AUTOSIZE); // Create a window for display.
	cv::imshow("Display window", diagonalMatrix);           // Show our image inside it.
	cv::waitKey(0);                                         // Wait for a keystroke in the window*/

	// Generate a salience mask
	maskGen.generateSalienceMask(reference, query, salienceMaskImage);
	writeFloatImage("C:\\LocalUser\\Documents\\Renders\\city dataset 2016-01-21\\salience mask.png", salienceMaskImage);
	std::cout << "Generated salience mask" << std::endl;

	/*cv::namedWindow("Display window", cv::WINDOW_AUTOSIZE); // Create a window for display.
	cv::imshow("Display window", salienceMaskImage);           // Show our image inside it.
	cv::waitKey(0);                                         // Wait for a keystroke in the window*/

	// Generate a final diagonal matrix using the salience mask.
	placerecog.generateDiagonalMatrix(reference, query, ThresholdSalienceMask(salienceMaskImage, 0.5), diagonalMatrix);
	writeFloatImage("C:\\LocalUser\\Documents\\Renders\\city dataset 2016-01-21\\diagonal matrix with mask.png", diagonalMatrix);
	std::cout << "Generated masked diagonal matrix" << std::endl;

	// Print the accuracy percentage for the final diagonal matrix
	//float performanceWithMask = placerecog.measurePerformance(diagonalMatrix, 1);
	std::cout << "Matching accuracy with salience mask: (" <<
		placerecog.measurePerformance(diagonalMatrix, 0) << ", " <<
		placerecog.measurePerformance(diagonalMatrix, 1) << ", " <<
		placerecog.measurePerformance(diagonalMatrix, 3) << ", " <<
		placerecog.measurePerformance(diagonalMatrix, 5) << ", " <<
		placerecog.measurePerformance(diagonalMatrix, 7) << ", " <<
		placerecog.measurePerformance(diagonalMatrix, 9) << ")" << std::endl;

	// Show the final diagonal matrix
	/*cv::namedWindow("Display window", cv::WINDOW_AUTOSIZE); // Create a window for display.
	cv::imshow("Display window", diagonalMatrix);           // Show our image inside it.
	cv::waitKey(0);                                         // Wait for a keystroke in the window*/

	std::system("pause");
}

// Main, do all the things.
int main(int argc, char* argv[]) {
	// Set up the image filters
	DownsampleFilter dsf(64, 64);
	GreyscaleFilter gf;
	std::list<ImageFilterInterface*> filters;
	filters.push_back(&dsf);
	filters.push_back(&gf);

	// Set up the image datasets
	CachedDataset* reference = buildReferenceDataset(filters);
	CachedDataset* query = buildQueryDataset(filters);
	std::cout << "Datasets loaded" << std::endl;

	// Run the experiment
	runExperiment(*reference, *query);
	
	// Clean up
	delete reference;
	delete query;

	return 0;
}


