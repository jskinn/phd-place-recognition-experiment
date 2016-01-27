/*
* ImageDataset.h
*
*  Created on: 26 Jan 2016
*      Author: john
*/

#ifndef IMAGEDATASET_H_
#define IMAGEDATASET_H_

#include <string>
#include <opencv2/core/core.hpp>

class ImageDataset
{
public:
	ImageDataset(std::string prefix, std::string suffix, int count, int initialIndex, int step, int pad);
	~ImageDataset();

	int count() const;
	bool get(int index, cv::Mat& output) const;

private:
	std::string prefix, suffix;
	int start, step, size, pad;

	virtual std::string getFilename(int index) const;
};

#endif /* IMAGEDATASET_H_ */



