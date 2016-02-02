/*
* CompositeDataset.cpp
*
*  Created on: 1 Feb 2016
*      Author: john
*/

#include "stdafx.h"
#include "CompositeDataset.h"


/*CompositeDataset::CompositeDataset(const std::list<ImageDatasetInterface*>& composedDatasets) :
	datasets(composedDatasets),
	_count(0)
{
	for (std::list<ImageDatasetInterface*>::const_iterator iter = composedDatasets.begin(); iter != composedDatasets.end(); ++iter) {
		_count += (*iter)->count();
	}
}


CompositeDataset::~CompositeDataset()
{
}

int CompositeDataset::count() const {
	return _count;
}

cv::Mat& CompositeDataset::get(int index) const {
	if (index >= 0 && index < this->_count) {
		return this->datasets.front()->get(index);
	}
	for (std::list<ImageDatasetInterface*>::const_iterator iter = datasets.begin(); iter != datasets.end(); ++iter) {
		int count = (*iter)->count();
		if (index < count) {
			return (*iter)->get(index);
		}
		else {
			index -= count;
		}
	}
}*/