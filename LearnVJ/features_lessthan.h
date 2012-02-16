#ifndef LEARNVJ_FEATURES_LESSTHAN_H
#define LEARNVJ_FEATURES_LESSTHAN_H

bool operator< (const Feature& left, const Feature& right) {
    // Order by type (in the order given in the enum)
    if(left->type < right->type) {
        return true;
    }
    if(right->type < left->type) {
        return false;
    }

    // Order by x1 -> y1 -> x2 -> y2 -> c1
    if(left->x1 < right->x1) { return true; } if(right->x1 < left->x1) { return false; }
    if(left->y1 < right->y1) { return true; } if(right->y1 < left->y1) { return false; }
    if(left->x2 < right->x2) { return true; } if(right->x2 < left->x2) { return false; }
    if(left->y2 < right->y2) { return true; } if(right->y2 < left->y2) { return false; }
    if(left->c1 < right->c1) { return true; } if(right->c1 < left->c1) { return false; }

    // Two rectangle features have no c2
    if(left->type == Feature::TWO_REC_HORIZ || left->type == Feature::TWO_REC_VERT) { return false; }

    // Order by c2
    if(left->c2 < right->c2) { return true; } if(right->c2 < left->c2) { return false; }

    // Completely equal
    return false;
}

#endif
