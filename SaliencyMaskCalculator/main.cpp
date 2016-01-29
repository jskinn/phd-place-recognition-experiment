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
#include "CachedDataset.h"
#include "PlaceRecognition.h"
#include "AverageDifferenceMaskGenerator.h"
#include "PairwiseSalienceMaskGenerator.h"
#include "ThresholdSalienceMask.h"

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

int main(int argc, char* argv[]) {
	// Set up the image filters
	DownsampleFilter dsf(64, 64);
	GreyscaleFilter gf;
	std::list<ImageFilterInterface*> filters;
	filters.push_back(&dsf);
	filters.push_back(&gf);

	// Set up the image datasets
	CachedDataset reference("C:\\LocalUser\\Documents\\Renders\\city dataset 2016-01-21\\x 14200\\MovieCapture_640x360_1.00 ", ".png", 600, 2, 1, 10, filters);
	CachedDataset query("C:\\LocalUser\\Documents\\Renders\\city dataset 2016-01-21\\x 14400\\MovieCapture_640x360_1.00 ", ".png", 600, 2, 1, 10, filters);
	std::cout << "Datasets loaded" << std::endl;

	// Set up the place recognition object, salience mask generator, and output image
	PlaceRecognition placerecog;
	PairwiseSalienceMaskGenerator maskGen(1);
	cv::Mat diagonalMatrix, salienceMaskImage;

	// generate an initial diagonal matrix without a salience mask
	placerecog.generateDiagonalMatrix(reference, query, diagonalMatrix);
	writeFloatImage("C:\\LocalUser\\Documents\\Renders\\city dataset 2016-01-21\\diagonal matrix no mask.png", diagonalMatrix);
	std::cout << "Created base diagonal matrix" << std::endl;

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

	// Show the final diagonal matrix
	/*cv::namedWindow("Display window", cv::WINDOW_AUTOSIZE); // Create a window for display.
	cv::imshow("Display window", diagonalMatrix);           // Show our image inside it.
	cv::waitKey(0);                                         // Wait for a keystroke in the window*/

	return 0;
}


