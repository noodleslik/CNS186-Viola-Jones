#include <stdio.h>
#include <opencv2/opencv.hpp>
using namespace cv;

#define NUM 7960
#define SIZE 25
/*
 * resize images into fix size
 */
int main(int argc, const char* argv[])
{
	int i;
	Point pt1, pt2;
	if(argc<2)
	{
		// ./conv ../24/BMP/nonfaces/
		printf("Usage: ./resize image_dir\n");
		return -1;
	}
	char path[256], filename[128];
	for(i=1; i<=NUM; i++)
	{
		strcpy(path, argv[1]);
		sprintf(filename, "nonface%04d.bmp", i);
		strcat(path, filename);
		printf("%s\n", path);
		Mat orig = imread(path, 0);
		Mat adp;
		equalizeHist(orig, adp);
		Mat dst;
		resize(adp, dst, Size(SIZE, SIZE), 0, 0);
	#if 0
		imshow("Image", dst);
		waitKey();
	#endif
		sprintf(path, "../Negative_Dataset/%d.jpg", i);
		imwrite(path, dst);
	}
}
