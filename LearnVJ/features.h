#ifndef LEARNVJ_FEATURES_H_
#define LEARNVJ_FEATURES_H_

#include "feature_type.h"
#include <opencv2/opencv.hpp>
#include <set>

using namespace std;
using namespace cv;

/*
 * Definition of a feature as well as various utility functions for them
 * (such as checking validity or generating random sets of them).
 */

const int SUBWINDOW_SIZE = 24;

struct Feature
{
	FeatureTypeT type;
	// Required for all feature types 
	// (UL, LR points of rectangle 1):
	int x1, y1; // Up Left
	int x2, y2; // Low Right
	// All remaining coordinates are determined since all rectangles are same size and shape 
	Feature() : x1(0), y1(0), x2(0), y2(0) {}
	Feature(Feature& f)
	{
		type = f.type;
		x1 = f.x1; x2 = f.x2;
		y1 = f.y1; y2 = f.y2;
	}
};

set<Feature*>* GenerateAllFeatures(int step);
// Generate a set of n features. If a non-default value of snap_to is provided
// Function will not check that
// num_features is smaller than the possible number of features, so if it is too big, the function will loop.
set<Feature*>* GenerateRandomFeatures(int num_features);

// Calculates a feature given the IntegralImage
int CalculateFeature(Feature* feature, const Mat& integral_image);

#endif

