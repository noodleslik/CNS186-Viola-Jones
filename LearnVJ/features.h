#ifndef LEARNVJ_FEATURES_H_
#define LEARNVJ_FEATURES_H_

#include "cv.h"

#include <set>

using namespace std;
using namespace cv;

/* Generic file that contains definition of a feature as well as various utility functions for them (such as
   checking validity or generating random sets of them).

*/

const int SUBWINDOW_SIZE = 25;

struct Feature {
    enum FeatureTypeT { TWO_REC_HORIZ, TWO_REC_VERT, THREE_REC_HORIZ, THREE_REC_VERT, FOUR_REC };
    FeatureTypeT type;
    // Required for all feature types 
    // (UL, LR points of rectangle 1):
    int x1, y1;
    int x2, y2;
    // All remaining coordinates are determined since all rectangles are same size and shape 
};

bool operator< (const Feature& left, const Feature& right);

// Valid features require that:
//   - x1 < x2 
//   - y1 < y2
//   - Rectangles don't come off screen
bool IsValidFeature(Feature* to_check);

// The text here is out of date.
// Generate a set of n features. If a non-default value of snap_to is provided
// Function will not check that
// num_features is smaller than the possible number of features, so if it is too big, the function will loop.
set<Feature*>* GenerateRandomFeatures(int num_features);

// Calculates a feature given the IntegralImage
int CalculateFeature(Feature* feature, Mat integral_image);

#endif
