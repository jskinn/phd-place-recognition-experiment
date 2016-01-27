/*
* ImageFilterInterface.h
*
*  Created on: 26 Jan 2016
*      Author: john
*/

#ifndef IMAGEFILTERINTERFACE_H_
#define IMAGEFILTERINTERFACE_H_

#include <opencv2/core/core.hpp>
#include <list>

/**
* An interface for classes that filter images, so that I can adjust the downsampling and such quickly.
*/
class ImageFilterInterface {
public:
	virtual ~ImageFilterInterface() {};

	virtual void filterImage(cv::Mat& image) const = 0;

	/**
	 * Apply a collection of filters to a single image.
	 * This is to save us from long lists of filterImage calls.
	 * Note that the parameter is a list of pointers, because it can't instansiate ImageFilterInterface.
	 * Just declare the filters and add their addresses to the list.
	 * Alternatively, I could make this a 
	 */
	static void applyFilters(cv::Mat& image, std::list<ImageFilterInterface*> filters) {
		for (std::list<ImageFilterInterface*>::const_iterator iter = filters.begin(); iter != filters.end(); ++iter) {
			(*iter)->filterImage(image);
		}
	}
};

#endif /* IMAGEFILTERINTERFACE_H_ */
