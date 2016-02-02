/*
* CachedDataset.cpp
*
*  Created on: 27 Jan 2016
*      Author: john
*/

#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "CachedDataset.h"


CachedDataset::CachedDataset(const std::list<ImageLoaderInterface*>& imageLoaders) :
	_count(0)
{
	// Go through all the image loaders to determine the total number of images to load.
	for (std::list<ImageLoaderInterface*>::const_iterator iter = imageLoaders.begin(); iter != imageLoaders.end(); ++iter) {
		this->_count += (*iter)->getCount();
	}

	// Allocate the vector
	this->_imageStorage = std::vector<DatasetImage*>(this->_count, NULL);

	// Load all the images
	int offset = 0;
	for (std::list<ImageLoaderInterface*>::const_iterator iter = imageLoaders.begin(); iter != imageLoaders.end(); ++iter) {
		(*iter)->loadImages(this->_imageStorage, offset);
		offset += (*iter)->getCount();
	}
}

CachedDataset::~CachedDataset()
{
	// Clean up all the images stored in this dataset.
	for (std::vector<DatasetImage*>::iterator iter = this->_imageStorage.begin(); iter != this->_imageStorage.end(); ++iter) {
		delete *iter;
	}
}

int CachedDataset::count() const {
	return this->_count;
}

DatasetImage& CachedDataset::get(int index) const {
	if (index >= 0 && index < this->_count) {
		return *this->_imageStorage[index];
	}
	else if (index < 0) {
		return *this->_imageStorage[0];
	}
	else {
		return *this->_imageStorage[this->_count - 1];
	}
}