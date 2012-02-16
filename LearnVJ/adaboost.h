#ifndef LEARNVJ_ADABOOST_H
#define LEARNVJ_ADABOOST_H

#include <pair>
#include <set>

using namespace std;

struct Feature;

struct AdaBoostFeature {
    Feature* feature;
    int threshold;
    int polarity; // -1 or 1
};

bool operator< (const AdaboostFeature& left, const AdaBoostFeature& right) {
    if(left->feature < right->feature) { return true; } if(right->feature < left->feature) { return false; }
    if(left->threshold < right->threshold) { return true; } if(right->threshold < left->threshold) { return false; }
    if(left->polarity < right->polarity) { return true; } if(right->polarity < left->polarity) { return false; }
    return false;
}

// Runs the overall adaboost algorithm, which_faces is the last face in the training set, which_not_faces is the last face in the 
// negative training set, how_many is the number of features to return.
set<AdaBoostFeature> RunAdaboost(int which_faces, int which_not_faces, int how_many);

// Runs one round of the adaboost algorithm (calculates errors, finds best features, returns thresh, feat, pol). 
// Also updates weightings.
AdaBoostFeature RunAdaboostRound(Mat integral_image, set<Feature*> feature_set);

//Given a set of positive and negative values, finds best threshold and polarity also returns error.
Pair< Pair<int, int>, double> FindThresholdAndPolarity(vector<int> positive_examples, vector<int> negative_examples, int feature_value);

#endif
