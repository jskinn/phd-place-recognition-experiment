#include "stdafx.h"
#include "SimilarityCriteria.h"


SimilarityCriteria::SimilarityCriteria(double radius) : radiusSquared(radius * radius)
{
}


SimilarityCriteria::~SimilarityCriteria()
{
}

bool SimilarityCriteria::isImageSimilar(const DatasetImage& baseImage, const DatasetImage& comparisonImage) const
{
	// Check distance
	cv::Vec3d baseLoc = baseImage.getLocation();
	cv::Vec3d compLoc = comparisonImage.getLocation();
	double diffX = baseLoc[DatasetImage::X_AXIS] - compLoc[DatasetImage::X_AXIS];
	double diffY = baseLoc[DatasetImage::Y_AXIS] - compLoc[DatasetImage::Y_AXIS];
	double diffZ = baseLoc[DatasetImage::Z_AXIS] - compLoc[DatasetImage::Z_AXIS];

	if (diffX * diffX + diffY * diffY + diffZ * diffZ <= this->radiusSquared) {

		// The position is close to the other position, check orientation
		// For now, check the orientations are approximately equal. In the future, should check forward and up vectors are both within a set angle of each other.
		cv::Vec3d rotA = baseImage.getOrientation();
		cv::Vec3d rotB = comparisonImage.getOrientation();
		if (std::abs(rotA[DatasetImage::X_AXIS] - rotB[DatasetImage::X_AXIS]) < 1 &&
			std::abs(rotA[DatasetImage::Y_AXIS] - rotB[DatasetImage::Y_AXIS]) < 1 &&
			std::abs(rotA[DatasetImage::Z_AXIS] - rotB[DatasetImage::Z_AXIS]) < 1) {
			return true;
		}
	}

	return false;
}
