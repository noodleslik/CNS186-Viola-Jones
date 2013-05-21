#include <stdio.h>
#include <opencv2/opencv.hpp>
using namespace cv;

#define SIZE 25
/*
 * extract face from each image sample
 */
int main(int argc, const char* argv[])
{
	FILE* fin;
	Point pt1, pt2;
	Scalar scalar(255, 255, 0, 0);
	if(argc<3)
	{
		// ./conv ../WebFaces_BBS.txt ../WebFaces/
		printf("Usage: ./conv filename image_dir\n");
		return -1;
	}
	
	fin = fopen(argv[1], "r"); // input text file
	assert(fin);
	char path[256], filename[128], temp[128];
	unsigned int x, y, x1, y1;
	while(!feof(fin))
	{
		strcpy(path, argv[2]); // image dir
		fscanf(fin, "%s %s %d %d %d %d", filename, temp, &x, &y, &x1, &y1);
		printf("%s %s %d %d %d %d\n", filename, temp, x, y, x1, y1);
		pt1.x = x;
		pt1.y = y;
		pt2.x = x1;
		pt2.y = y1;
		strcat(path, temp);
		Mat image;
		image = imread(path, 0);
		Mat roi(image, Rect(pt1, pt2));
		Mat dst;
		resize(roi, dst, Size(SIZE, SIZE), 0, 0);
		strcpy(path, "../Faces_Normalized/");
		strcat(path, filename);
		imwrite(path, dst);
	#if 0
		rectangle(image, pt1, pt2, scalar, 2, 8, 0);
		imshow("Image", dst);
		waitKey();
	#endif
	}
	return 0;
}

