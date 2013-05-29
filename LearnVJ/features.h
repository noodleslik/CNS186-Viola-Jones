#ifndef LEARNVJ_FEATURES_H_
#define LEARNVJ_FEATURES_H_

#include "feature_type.h"
#include "../Array/array.hpp"
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

struct Feature;

const int SUBWINDOW_SIZE = 24;

// Generate all possible features
list<Feature*>* GenerateAllFeatures(int step);
// Function will not check that num_features is smaller than
// the possible number of features, the function will loop.
list<Feature*>* GenerateRandomFeatures(int num_features);

// Calculates a feature by the given IntegralImage
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
	void calculate_pos_results(const array<Mat> &pos_iis)
	{
		for(size_t i = 0; i != pos_iis.size(); ++i)
			positive_results.push_back(CalculateFeature(this, pos_iis[i]));
	}
	void calculate_neg_results(const array<Mat> &neg_iis)
	{
		for(size_t i = 0; i != neg_iis.size(); ++i)
			negative_results.push_back(CalculateFeature(this, neg_iis[i]));
	}
};

#endif

