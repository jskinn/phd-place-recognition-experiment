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
#include "ThresholdSalienceMask.h"

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

	// Set up the place recognition object, salience mask generator, and output image
	PlaceRecognition placerecog;
	AverageDifferenceMaskGenerator maskGen;
	cv::Mat diagonalMatrix, salienceMaskImage;

	// generate an initial diagonal matrix without a salience mask
	placerecog.generateDiagonalMatrix(reference, query, diagonalMatrix);
	cv::imwrite("C:\\LocalUser\\Documents\\Renders\\city dataset 2016-01-21\\diagonal matrix no mask.png", diagonalMatrix);

	// Generate a salience mask
	maskGen.generateSalienceMask(reference, query, salienceMaskImage);
	cv::imwrite("C:\\LocalUser\\Documents\\Renders\\city dataset 2016-01-21\\salience mask.png", salienceMaskImage);

	// Generate a final diagonal matrix using the salience mask.
	placerecog.generateDiagonalMatrix(reference, query, ThresholdSalienceMask(salienceMaskImage), diagonalMatrix);
	cv::imwrite("C:\\LocalUser\\Documents\\Renders\\city dataset 2016-01-21\\diagonal matrix with mask.png", diagonalMatrix);

	//cv::namedWindow("Display window", cv::WINDOW_AUTOSIZE); // Create a window for display.
	//cv::imshow("Display window", diagonalMatrix);           // Show our image inside it.
	//cv::waitKey(0);                                         // Wait for a keystroke in the window
	return 0;
}


