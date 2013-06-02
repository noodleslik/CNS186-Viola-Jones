#ifndef LEARNVJ_ADABOOST_H
#define LEARNVJ_ADABOOST_H

#include "../Array/array.hpp"
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

//#define _NUM_THREADS 4

struct Feature;

struct AdaBoostFeature
{
	Feature* feature;
	int threshold;
	int polarity; // -1 or 1, 极性
	double beta_t;
	double false_pos_rate;
	// internal usage
	double error_rate;
	size_t feature_idx;
};
// Load objects
size_t LoadImage(unsigned int which_objs, unsigned int which_not_objs, 
                 array<Mat> &pos_iis, array<Mat> &neg_iis);

// which_objs: size of positive training set
// which_not_objs: size of negative training set
// how_many: number of features to return
// total_set: how many random feature to generate.
vector<AdaBoostFeature*> RunAdaBoost(unsigned int which_objs, unsigned int which_not_objs,
                                     unsigned int how_many, unsigned int total_set);

// Runs one round of the adaboost algorithm (calculates errors, finds best features, returns thresh, feat, pol). 
// Also updates weightings. Modifies weightings correctly and removes the selected feature from the feature set.
AdaBoostFeature* RunAdaBoostRound(const array<Mat> &pos_iis, const array<Mat> &neg_iis,
                                  array<double> &pos_weights, array<double> &neg_weights,
                                  array<Feature*> *all_features);

// Given a set of positive and negative values of a particular feature,Puts the best
// threshold and polarity as well as total error based on weightings into the given vars
void FindThresholdAndPolarity(const array<int> &positive_examples, const array<int> &negative_examples,
                              const array<double> &pos_weights, const array<double> &neg_weights,
                              int* threshold, int* polarity, double* error, double* false_pos_rate);

// Save weak classifiers to file
void SaveAdaBoost(const vector<AdaBoostFeature*> to_save, const char* filename); 

#endif

