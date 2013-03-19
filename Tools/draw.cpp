#include <stdio.h>
#include <opencv2/opencv.hpp>
using namespace cv;

/*
 * draw Caltech 10k WebFaces dataset of each image
 */
int main(int argc, const char* argv[])
{
	FILE* fin;
	Point pt1, pt2, pt3, pt4;
	Scalar scalar(255, 255, 0, 0);
	if(argc<3)
	{
		// ./draw ../WebFaces_GroundThruth.txt ../WebFaces/
		printf("Usage: ./draw filename image_dir\n");
		return -1;
	}
	
	fin = fopen(argv[1], "r"); // input text file
	assert(fin);
	char path[256], filename[128];
	double le_x, le_y, re_x, re_y; // two eye
	double n_x, n_y, m_x, m_y; // nose & mouth
	while(!feof(fin))
	{
		strcpy(path, argv[2]); // image dir
		fscanf(fin, "%s %lf %lf %lf %lf", filename, &le_x, &le_y, &re_x, &re_y);
		fscanf(fin, "%lf %lf %lf %lf", &n_x, &n_y, &m_x, &m_y);
		strcat(path, filename);
		printf("%s:\n%lf %lf %lf %lf\n", path, le_x, le_y, re_x, re_y);
		printf("%lf %lf %lf %lf\n", n_x, n_y, m_x, m_y);
		pt1.x = le_x;	pt1.y = le_y;
		pt2.x = re_x;	pt2.y = re_y;
		pt3.x = n_x;	pt3.y = n_y;
		pt4.x = m_x;	pt4.y = m_y;
		Mat image = imread(path, 1);
		circle(image, pt1, 1, scalar, 2, 8, 0);
		circle(image, pt2, 1, scalar, 2, 8, 0);
		circle(image, pt3, 1, scalar, 2, 8, 0);
		circle(image, pt4, 1, scalar, 2, 8, 0);
		imshow("result", image);
		waitKey();
	}
	return 0;
}
