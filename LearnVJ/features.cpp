#include "features.h"

#include <cstdlib>
#include <ctime>

using namespace std;
using namespace cv;

bool operator< (const Feature& left, const Feature& right) {
    // Order by type (in the order given in the enum)
    if(left.type < right.type) {
        return true;
    }
    if(right.type < left.type) {
        return false;
    }

    // Order by x1 -> y1 -> x2 -> y2 -> c1
    if(left.x1 < right.x1) { return true; } if(right.x1 < left.x1) { return false; }
    if(left.y1 < right.y1) { return true; } if(right.y1 < left.y1) { return false; }
    if(left.x2 < right.x2) { return true; } if(right.x2 < left.x2) { return false; }
    if(left.y2 < right.y2) { return true; } if(right.y2 < left.y2) { return false; }

    // Completely equal
    return false;
}

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

set<Feature*>* GenerateRandomFeatures(int num_features) {
    set<Feature*>* storage = new set<Feature*>();
    srand(time(NULL));
    for(int i=0; i<num_features; ++i) {
        int lowest = 0; int highest = 4; int range = (highest - lowest) + 1;
        int type = lowest + int(range * rand()/(RAND_MAX + 1));
        int x1,y1,x2,y2;
        if(type == 0 || type == 1) {
            lowest = 0; highest = SUBWINDOW_SIZE - 1 - 2; range = (highest - lowest) + 1;
            x1 = lowest + int(range * rand()/(RAND_MAX + 1));
            lowest = 0; highest = SUBWINDOW_SIZE - 1 - 1; range = (highest - lowest) + 1;
            y1 = lowest + int(range * rand()/(RAND_MAX + 1));
            lowest = 1; highest = ((SUBWINDOW_SIZE - 1) - x1) / 2; range = (highest - lowest) + 1; 
            int x_diff = lowest + int(range * rand()/(RAND_MAX + 1));
            x2 = x1 + x_diff;
            lowest = y1 + 1; highest = SUBWINDOW_SIZE - 1; range = (highest - lowest) + 1;
            y2 = lowest + int(range * rand()/(RAND_MAX + 1)); 
        } // For type = 1 we will simply reverse x and y
        else if(type == 2 || type == 3) { 
            lowest = 0; highest = SUBWINDOW_SIZE - 1 - 3; range = (highest - lowest) + 1;
            x1 = lowest + int(range * rand()/(RAND_MAX + 1));
            lowest = 0; highest = SUBWINDOW_SIZE - 1 - 1; range = (highest - lowest) + 1;
            y1 = lowest + int(range * rand()/(RAND_MAX + 1));
            lowest = 1; highest = ((SUBWINDOW_SIZE - 1) - x1) / 3; range = (highest - lowest) + 1; 
            int x_diff = lowest + int(range * rand()/(RAND_MAX + 1));
            x2 = x1 + x_diff;
            lowest = y1 + 1; highest = SUBWINDOW_SIZE - 1; range = (highest - lowest) + 1;
            y2 = lowest + int(range * rand()/(RAND_MAX + 1)); 
        } // For type = 3 we will simply reverse x and y
        else if(type == 4) { 
            lowest = 0; highest = SUBWINDOW_SIZE - 1 - 2; range = (highest - lowest) + 1;
            x1 = lowest + int(range * rand()/(RAND_MAX + 1));
            lowest = 0; highest = SUBWINDOW_SIZE - 1 - 2; range = (highest - lowest) + 1;
            y1 = lowest + int(range * rand()/(RAND_MAX + 1));
            lowest = 1; highest = ((SUBWINDOW_SIZE - 1) - x1) / 2; range = (highest - lowest) + 1; 
            int x_diff = lowest + int(range * rand()/(RAND_MAX + 1));
            lowest = 1; highest = ((SUBWINDOW_SIZE - 1) - y1) / 2; range = (highest - lowest) + 1;
            int y_diff = lowest + int(range * rand()/(RAND_MAX + 1));
            x2 = x1 + x_diff;
            y2 = y1 + y_diff;
        }
        if(type == 1 || type == 3) { int temp = x1; x1 = y1; y1 = temp; temp = x2; x2 = y2; y2 = x2; }
        Feature* new_creation = new Feature();
        new_creation->x1 = x1;
        new_creation->x2 = x2;
        new_creation->y1 = y1;
        new_creation->y2 = y2;
        if(type == 0) { new_creation->type = Feature::TWO_REC_HORIZ; }
        else if(type == 1) { new_creation->type = Feature::TWO_REC_VERT; }
        else if(type == 2) { new_creation->type = Feature::THREE_REC_HORIZ; }
        else if(type == 3) { new_creation->type = Feature::THREE_REC_VERT; }
        else { new_creation->type = Feature::FOUR_REC; }

        if(!IsValidFeature(new_creation)) { return NULL; }

        storage->insert(new_creation);
    }
    return storage;
}

int CalculateFeature(Feature* feature, Mat integral_image) {
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
    if(feature->type == Feature::TWO_REC_VERT || feature->type == Feature::THREE_REC_VERT ||
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

    return (int)current_sum;
}
