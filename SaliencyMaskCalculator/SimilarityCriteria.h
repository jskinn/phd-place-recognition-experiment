/*
* SimilarityCriteria.h
*
*  Created on: 29 Jan 2016
*      Author: john
*/

#ifndef SIMILARITYCRITERIA_H_
#define SIMILARITYCRITERIA_H_

#include "DatasetImage.h"

class SimilarityCriteria
{
public:
	SimilarityCriteria(double radius);
	virtual ~SimilarityCriteria();

	virtual bool isImageSimilar(const DatasetImage& baseImage, const DatasetImage& comparisionImage) const;

private:
	double radiusSquared;
};

#endif /* SIMILARITYCRITERIA_H_ */
