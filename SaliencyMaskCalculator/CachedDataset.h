/*
* CachedDataset.h
*
*  Created on: 27 Jan 2016
*      Author: john
*/

#ifndef CACHEDDATASET_H_
#define CACHEDDATASET_H_

#include <vector>
#include <list>
#include "ImageLoaderInterface.h"
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
	CachedDataset(const std::list<ImageLoaderInterface*>& imageLoaders);
	virtual ~CachedDataset();

	virtual int count() const;
	virtual DatasetImage& get(int index) const;

private:
	std::vector<DatasetImage*> _imageStorage;
	int _count;
};

#endif /* CACHEDDATASET_H_ */