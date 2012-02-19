#include "features.h"

using namespace std;
using namespace cv;

// Requirements are listed in the header file.
bool IsValidFeature(Feature* to_check) {
    if(to_check->x1 > to_check->x2) { return false; }
    if(to_check->y1 > to_check->y2) { return false; }
    if(to_check->x1 < 0 || to_check->x2 >= SUBWINDOW_SIZE) { return false; }
    if(to_check->y1 < 0 || to_check->y2 >= SUBWINDOW_SIZE) { return false; }
    if((to_check->type == Feature::TWO_REC_HORIZ || to_check->type == Feature::THREE_REC_HORIZ || 
        to_check->type == Feature::FOUR_REC) && ((to_check->x2 + (to_check->x2 - to_check->x1)) >= SUBWINDOW_SIZE)) {
        return false;
    }
    if((to_check->type == Feature::TWO_REC_VERT || to_check->type == Feature::THREE_REC_VERT || 
        to_check->type == Feature::FOUR_REC) && ((to_check->y2 + (to_check->y2 - to_check->y1)) >= SUBWINDOW_SIZE)) {
        return false;
    }
    if((to_check->type == Feature::THREE_REC_HORIZ) && 
            ((to_check->x2 + 2*(to_check->x2 - to_check->x1)) >= SUBWINDOW_SIZE)) {
        return false;
    }
    if((to_check->type == Feature::THREE_REC_VERT) && 
            ((to_check->x2 + 2*(to_check->y2 - to_check->y1)) >= SUBWINDOW_SIZE)) {
        return false;
    }
    return true;
}

set<Feature*> GenerateRandomFeatures(int window_size, int num_features, int snap_to = 1) {

}

double CalculateFeature(Feature* feature, Mat integral_image) {
    double current_sum = 0;
    if(!IsValidFeature(feature)) { return -1; }
    // First rectangle is the same for all.
    current_sum += integral_image.at<double>(feature->y1, feature->x1) + 
                   integral_image.at<double>(feature->y2, feature->x2) -
                   integral_image.at<double>(feature->y1, feature->x2) -
                   integral_image.at<double>(feature->y2, feature->x1);
    // Second rectangle for horizontal 2 and 3  or 4
    if(feature->type == Feature::TWO_REC_HORIZ || feature->type == Feature::THREE_REC_HORIZ || 
            feature->type == Feature::FOUR_REC) {
        int second_x = feature->x2 + (feature->x2 - feature->x1);
        current_sum -= integral_image.at<double>(feature->y1, feature->x2) +
                       integral_image.at<double>(feature->y2, second_x) -
                       integral_image.at<double>(feature->y1, second_x) -
                       integral_image.at<double>(feature->y2, feature->x2);
    }
    // Second rectangle for vertical 2 and 3, third rectangle for 4
    if(feature->type == Feature::TWO_REC_VERT || feature->type == Feature->THREE_REC_VERT ||
            feature->type == Feature::FOUR_REC) {
        int second_y = feature->y2 + (feature->y2 - feature->y1);
        current_sum -= integral_image.at<double>(feature->y2, feature->x1) +
                       integral_image.at<double>(second_y, feature->x2) -
                       integral_image.at<double>(feature->y2, feature->x2) -
                       integral_image.at<double>(second_y, feature->x1);
    }
    // Third rectangle for horizontal 3
    if(feature->type == Feature::THREE_REC_HORIZ) {
        int second_x = feature->x2 + (feature->x2 - feature->x1);
        int third_x = feature->x2 + 2*(feature->x2 - feature->x1);
        current_sum += integral_image.at<double>(feature->y1, second_x) +
                       integral_image.at<double>(feature->y2, third_x) -
                       integral_image.at<double>(feature->y1, third_x) -
                       integral_image.at<double>(feature->y2, second_x);
    }
    // Third rectangle for vertical 3
    if(feature->type == Feature::THREE_REC_VERT) {
        int second_y = feature->y2 + (feature->y2 - feature->y1);
        int third_y = feature->y2 + 2*(feature->y2 - feature->y1);
        current_sum += integral_image.at<double>(second_y, feature->x1) +
                       integral_image.at<double>(third_y, feature->x2) -
                       integral_image.at<double>(second_y, feature->x2) -
                       integral_image.at<double>(third_y, feature->x1);
    }
    // Fourth rectangle for 4
    if(feature->type == Feature::FOUR_REC) {
        int x1 = feature->x2;
        int y1 = feature->y2;
        int x2 = feature->x2 + (feature->x2 - feature->x1);
        int y2 = feature->y2 + (feature->y2 - feature->y1);
        current_sum += integral_image.at<double>(y1, x1) + 
                       integral_image.at<double>(y2, x2) -
                       integral_image.at<double>(y1, x2) -
                       integral_image.at<double>(y2, x1);
    }

    return current_sum;
}
