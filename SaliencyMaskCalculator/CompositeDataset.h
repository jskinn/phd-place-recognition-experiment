/*
* CompositeDataset.h
*
*  Created on: 01 Feb 2016
*      Author: john
*/

#ifndef COMPOSITEDATASET_H_
#define COMPOSITEDATASET_H_

#include <list>
#include "ImageDatasetInterface.h"

/**
 * A Dataset composed of other datasets.
 * It unifies the indexing and lets the combined dataset be used together.
 * Key disadvantage is that index time is now O(n) where n is the number of composed datasets.
 * TODO: This class is obsolete now that the ImageLoader separates out the loading, allowing one dataset to load several sets of images.
 */
/*class CompositeDataset : public ImageDatasetInterface
{
public:
	CompositeDataset(const std::list<ImageDatasetInterface*>& composedDatasets);
	virtual ~CompositeDataset();

	virtual int count() const;
	virtual cv::Mat& get(int index) const;

private:
	std::list<ImageDatasetInterface*> datasets;
	int _count;
};*/

#endif /* COMPOSITEDATASET_H_ */