/*
* CachedDataset.h
*
*  Created on: 27 Jan 2016
*      Author: john
*/

#ifndef CACHEDDATASET_H_
#define CACHEDDATASET_H_

#include <list>
#include "ImageFilterInterface.h"
#include "ImageDatasetInterface.h"

/**
* CachedDataset, a dataset wrapper that preloads all the images in the dataset.
* Space can be significantly saved by specifying a filter list, which can reduce the size of the stored images.
*
* TODO: Add a random-access iterator to this.
*/
class CachedDataset : public ImageDatasetInterface
{
public:
	CachedDataset(std::string prefix, std::string suffix, int count, int initialIndex, int step, int pad, const std::list<ImageFilterInterface*>& filters = std::list<ImageFilterInterface*>());
	virtual ~CachedDataset();

	virtual int count() const;
	virtual cv::Mat& get(int index) const;

private:
	cv::Mat* imageStorage;
	int _count;
};

#endif /* CACHEDDATASET_H_ */