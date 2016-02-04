/*
* VPRiCELoader.h
*
*  Created on: 2 Feb 2016
*      Author: john
*/

#ifndef VPRICELOADER_H_
#define VPRICELOADER_H_

#include <string>
#include <list>
#include "ImageLoaderInterface.h"
#include "ImageFilterInterface.h"

/**
 * A loader for the VPRiCE dataset, used in the lab.
 * Not the most suitable dataset (SAD match is pretty low due to difference in conditions).
 * 
 * Note that the dataset is actually comprised of a number of sub-datasets, the 
 * sub-dataset boundaries are at indexes 2848-2485, 2528-2529, 2569-2570, 2653-2654, 2684-2685, 2728-2729, 2884-2885 and later
 * 
 * It's also clear that the dataset undergoes large orientation change, and we don't actually have recorded ground truth for that.
 */
class VPRiCELoader : public ImageLoaderInterface
{
public:
	VPRiCELoader(std::string folderPath, int imageCount, int initialIndex = 0, int step = 1, const std::list<ImageFilterInterface*>& filters = std::list<ImageFilterInterface*>());
	virtual ~VPRiCELoader();

	virtual int getCount();

	virtual void loadImages(std::vector<DatasetImage*>& destination, int indexOffset);

private:
	std::string folderPath;
	int imageCount;
	int initialIndex;
	int step;
	const std::list<ImageFilterInterface*>& filters;
};

#endif /* VPRICELOADER_H_ */
