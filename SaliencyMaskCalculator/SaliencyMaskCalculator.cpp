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
#include "ImageDataset.h"
#include "PlaceRecognition.h"

/*std::string getReferenceFileName(int index) {
	index += 2;
	std::stringstream s;
	s << "C:\\LocalUser\\Documents\\Renders\\city dataset 2016-01-21\\x 14200\\MovieCapture_640x360_1.00 ";
	if (index < 10) {
		s << '0';
	}
	s << index << ".png";
	return s.str();
}

std::string getQueryFileName(int index) {
	index += 2;
	std::stringstream s;
	s << "C:\\LocalUser\\Documents\\Renders\\city dataset 2016-01-21\\x 15200\\MovieCapture_640x360_1.00 ";
	if (index < 10) {
		s << '0';
	}
	s << index << ".png";
	return s.str();
}*/

int main(int argc, char* argv[]) {
	// Set up the image datasets
	ImageDataset reference("C:\\LocalUser\\Documents\\Renders\\city dataset 2016-01-21\\x 14200\\MovieCapture_640x360_1.00 ", ".png", 600, 2, 1, 10);
	ImageDataset query("C:\\LocalUser\\Documents\\Renders\\city dataset 2016-01-21\\x 14400\\MovieCapture_640x360_1.00 ", ".png", 600, 2, 1, 10);

	// Set up the image filters
	DownsampleFilter dsf(256, 256);
	GreyscaleFilter gf;
	std::list<ImageFilterInterface*> filters;
	filters.push_back(&dsf);
	filters.push_back(&gf);

	// Set up the place recognition object and output image
	PlaceRecognition placerecog;
	cv::Mat salienceMask;

	placerecog.generateSalienceMask(reference, query, salienceMask, filters);

	cv::imwrite("C:\\LocalUser\\Documents\\Renders\\city dataset 2016-01-21\\saliency mask.png", salienceMask);

	cv::namedWindow("Display window", cv::WINDOW_AUTOSIZE); // Create a window for display.
	cv::imshow("Display window", salienceMask);                  // Show our image inside it.
	cv::waitKey(0);                                         // Wait for a keystroke in the window
	return 0;

	/*cv::Mat diffImages[600];
	double imageScores[600];
	double sadSum(0.0);
	cv::Mat avgDiff(cv::Mat::zeros(256, 256, CV_32F));
	int index;
	cv::Mat referenceImage, queryImage;
	std::string reference, query;
	DownsampleFilter dsf(256, 256);
	GreyscaleFilter gf;

	for (index = 0; index < 600; ++index) {
		cv::Mat referenceImage, queryImage;
		reference = getReferenceFileName(index);
		query = getQueryFileName(index);

		referenceImage = cv::imread(reference.c_str(), cv::IMREAD_COLOR); // Read the reference image
		if (referenceImage.empty())                                      // Check for invalid input
		{
			std::cout << "Could not open or find the reference image: " << reference << std::endl;
			continue;
		}

		queryImage = cv::imread(query.c_str(), cv::IMREAD_COLOR); // Read the query image
		if (queryImage.empty())                                  // Check for invalid input
		{
			std::cout << "Could not open or find the query image: " << query << std::endl;
			continue;
		}

		dsf.filterImage(referenceImage);
		gf.filterImage(referenceImage);
		dsf.filterImage(queryImage);
		gf.filterImage(queryImage);

		cv::absdiff(referenceImage, queryImage, diffImages[index]);

		imageScores[index] = cv::sum(diffImages[index])[0];
		sadSum += imageScores[index];

		cv::add(avgDiff, diffImages[index], avgDiff, cv::noArray(), CV_32F);
	}
	sadSum /= 600.0;
	avgDiff = avgDiff * (1.0 / 600.0);

	imwrite("C:\\LocalUser\\Documents\\Renders\\city dataset 2016-01-21\\saliency mask.png", avgDiff);

	cv::namedWindow("Display window", cv::WINDOW_AUTOSIZE); // Create a window for display.
	cv::imshow("Display window", avgDiff);                  // Show our image inside it.
	cv::waitKey(0);                                         // Wait for a keystroke in the window
	return 0;*/
}


