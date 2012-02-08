#include "integral_image.h"

#include "cv.h"
#include <iostream>
#include <stdint.h>

using namespace std;

int main(int argc, char** argv) {
    IplImage* img;
    if(argc == 2 && (img = cvLoadImage(argv[1], 1)) != 0) {
        cv::Mat ii = integral_image(img);
    }
    return -1;
}
