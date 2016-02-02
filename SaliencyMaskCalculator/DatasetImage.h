/*
* DatasetImage.h
*
*  Created on: 1 Feb 2016
*      Author: john
*/

#ifndef DATASETIMAGE_H_
#define DATASETIMAGE_H_

#include <opencv2/core/core.hpp>

/**
 * A class that wraps an image and the ground truth about the location where the image came from.
 * This is useful so that we can combine datasets and not rely on the indexing to reflect ground truth.
 */
class DatasetImage
{
public:
	DatasetImage(cv::Mat& image, cv::Vec3d& location, cv::Vec3d& rotation);
	~DatasetImage();

	virtual const cv::Mat& getImage() const;
	virtual const cv::Vec3d& getLocation() const;
	virtual const cv::Vec3d& getOrientation() const;

	static const int X_AXIS = 0;
	static const int Y_AXIS = 1;
	static const int Z_AXIS = 2;

private:
	cv::Mat _image;
	cv::Vec3d _location;
	cv::Vec3d _rotation;
};

#endif /* DATASETIMAGE_H_ */