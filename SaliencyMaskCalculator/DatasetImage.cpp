#include "stdafx.h"
#include "DatasetImage.h"


DatasetImage::DatasetImage(cv::Mat& image, cv::Vec3d& location, cv::Vec3d& rotation) :
	_image(image),
	_location(location),
	_rotation(rotation)
{
}


DatasetImage::~DatasetImage()
{
}


const cv::Mat& DatasetImage::getImage() const
{
	return this->_image;
}


const cv::Vec3d& DatasetImage::getLocation() const
{
	return this->_location;
}


const cv::Vec3d& DatasetImage::getOrientation() const
{
	return this->_rotation;
}
