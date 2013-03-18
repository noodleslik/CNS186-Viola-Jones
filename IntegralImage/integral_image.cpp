#include "cv.h"
#include "highgui.h"
#include <iostream>
#include <stdint.h>

using namespace std;
using namespace cv;

Mat IntegralImage(Mat img)
{
	// Image should have 8-bits because I am lazy.
	if(img.type() != CV_8UC1) { 
		return Mat::zeros(0, 0, CV_64F); 
	}

	// Allocate S and ii matrices.
	Mat S_prime = img.clone();
	Mat S;
	S_prime.convertTo(S, CV_64F);

	for(int i = 1; i < img.rows; ++i) {
		S.row(i) = S.row(i) + S.row(i-1);
	}

	Mat ii = S.clone();
	for(int i = 1; i < img.cols; ++i) {
		ii.col(i) = ii.col(i) + ii.col(i-1);
	}

	if(ii.data == NULL) { 
		cout << "WARNING: Integral image is null." << endl;
	}

	return ii;
}
