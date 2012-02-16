#ifndef LEARNVJ_FEATURES_H_
#define LEARNVJ_FEATURES_H_

#include "cv.h"
#include "features_lessthan.h"

#include <set>

using namespace std;
using namespace cv;

/* Generic file that contains definition of a feature as well as various utility functions for them (such as
   checking validity or generating random sets of them).

*/

struct Feature {
    enum FeatureTypeT { TWO_REC_HORIZ, TWO_REC_VERT, THREE_REC_HORIZ, THREE_REC_VERT, FOUR_REC };
    FeatureTypeT type;
    // Required for all feature types 
    // (UL, LR points of rectangle 1):
    int x1, y1;
    int x2, y2;
    // Remaining determining coordinate for rectangle 2 (x for horiz 2,3 or 4, y for vert 2,3)
    int c1;
    // Required only for 3 and 4:
    // Remaining determining coordinate for rectangle 3 (x for horiz 3, y for vert 3 or 4)
    int c2;
};

// Valid features require that:
//   - x1 < x2 
//   - y1 < y2
//   - x2 < c1 for horiz 2,3 or 4  y2 < c1 for vert 2,3
//   - x2 < c2 for horiz 3         y2 < c2 for vert 3 or 4
bool IsValidFeature(Feature* to_check);

// Generate a set of n features. If a non-default value of snap_to is provided, features will only use multiples
// of snap_to as their keypoints. snap_to must obviously be less than window_size. Function will not check that
// num_features is smaller than the possible number of features, so if it is too big, the function will loop.
set<Feature*> GenerateRandomFeatures(int window_size, int num_features, int snap_to = 1);

// Calculates a feature given the IntegralImage
int CalculateFeature(Feature* feature, Mat integral_image);

#endif
