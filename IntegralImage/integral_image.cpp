#include "cv.h"
#include "highgui.h"
#include <iostream>
#include <stdint.h>

using namespace std;

cv::Mat intgral_image(IplImage* img) {
    // Image should have 8-bits because I am lazy.
    if(img->depth != IPL_DEPTH_8U) { 
        return Mat::zeros(0, 0, CV_64UC1); 
    }

    // Allocate S and ii matrices.
    cv::Mat im(img, true);
    cv::Mat S(img, true);
    Mat::convertTo(&S, CV_64UC1);

    for(int i = 1; i < im.rows; ++i) {
        S.row(i) = S.row(i) + S.row(i-1);
    }

    cv::Mat ii = S.clone();
    for(int i = 1; i < im.cols; ++i) {
        ii.col(i) = ii.col(i) + ii.col(i-1);
    }

    return ii;
}
