/*
* NullSalienceMask.h
*
*  Created on: 28 Jan 2016
*      Author: john
*/

#ifndef NULLSALIENCEMASK_H_
#define NULLSALIENCEMASK_H_

#include "SalienceMaskInterface.h"

/**
 * A null salience mask. Doesn't mask anything, and takes next to no memory.
 */
class NullSalienceMask : SalienceMaskInterface
{
public:
	NullSalienceMask();
	virtual ~NullSalienceMask();

	virtual void applyMask(cv::Mat& image) const;
};

#endif /* NULLSALIENCEMASK_H_ */

