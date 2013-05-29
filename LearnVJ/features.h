#ifndef LEARNVJ_FEATURES_H_
#define LEARNVJ_FEATURES_H_

#include "feature_type.h"
#include "../Array/array.hpp"
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

/*
 * Definition of a feature as well as various utility functions for them
 * (such as checking validity or generating random sets of them).
 */

struct Feature;

const int SUBWINDOW_SIZE = 24;

list<Feature*>* GenerateAllFeatures(int step);
// Generate a set of n features. If a non-default value of snap_to is provided
// Function will not check that
// num_features is smaller than the possible number of features, so if it is too big, the function will loop.
list<Feature*>* GenerateRandomFeatures(int num_features);

// Calculates a feature given the IntegralImage
int CalculateFeature(Feature* feature, const Mat& integral_image);

struct Feature
{
	FeatureTypeT type;
	// (UL, LR points of rectangle 1)
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
	array<int> positive_results; // feature value of positive samples
	array<int> negative_results; // feature value of negative samples
	void calculate_pos_results(const vector<Mat> &pos_iis)
	{
		vector<Mat>::const_iterator im_it;
		for(im_it = pos_iis.begin(); im_it != pos_iis.end(); ++im_it)
			positive_results.push_back(CalculateFeature(this, *im_it));
	}
	void calculate_neg_results(const vector<Mat> &neg_iis)
	{
		vector<Mat>::const_iterator im_it;
		for(im_it = neg_iis.begin(); im_it != neg_iis.end(); ++im_it)
			negative_results.push_back(CalculateFeature(this, *im_it));
	}
};

#endif

