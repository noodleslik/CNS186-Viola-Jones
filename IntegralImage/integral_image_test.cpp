#include "integral_image.h"

#include "cv.h"
#include "highgui.h"
#include <iostream>
#include <stdint.h>

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
    Mat img = imread(argv[1], 1);
    Mat gray_img;
    cvtColor(img, gray_img, CV_RGB2GRAY); 
    if(argc == 2 && img.data != NULL) {
        Mat ii = integral_image(gray_img);
        cout << ii << endl;
        cout << ii.at<double>(0, 0) << " " << ii.at<double>(5,5) << endl;
    }
    return -1;
}
