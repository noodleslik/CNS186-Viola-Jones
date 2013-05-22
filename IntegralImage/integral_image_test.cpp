#include "integral_image.h"

#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdint.h>

using namespace std;
using namespace cv;

int main(int argc, char* argv[])
{
	if(argc < 2)
		return 1;
	Mat gray_img = imread(argv[1], 0);
	if(gray_img.data != NULL)
	{
		Mat ii = IntegralImage(gray_img);
		cout << ii << endl;
		cout << ii.at<double>(0, 0) << " " << ii.at<double>(5,5) << endl;
	}
	return 0;
}
