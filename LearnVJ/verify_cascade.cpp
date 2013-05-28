#include "adaboost.h"
#include "features.h"
#include "../IntegralImage/integral_image.h"
#include <cstdio>

extern
void load_afeatures(vector<AdaBoostFeature*>& first_set, const char *file);
extern
void verify(const vector<AdaBoostFeature*> &afeatures, const Mat &iimg, vector<Point> &pnts);

int main(int argc, char *argv[])
{
	if(argc < 3)
	{
		printf("Usage: %s image stages\n", argv[0]);
		return 1;
	}
	
	Mat orig;
	orig = imread(argv[1], 0);
	int img_width = orig.cols;
	int img_height = orig.rows;
	while(img_width > 3*SUBWINDOW_SIZE && img_height > 3*SUBWINDOW_SIZE)
	{
		vector<Point> points;
		vector<AdaBoostFeature*> afeatures;
		Mat img, iimg;
		printf("At %d x %d\n", img_width, img_height);
		// get integral image
		resize(orig, img, Size(img_width, img_height));
		iimg = IntegralImage(img);
		// calculate all possible wins
		for(int x = 0; x < img_width - SUBWINDOW_SIZE; x++)
			for(int y = 0; y < img_height - SUBWINDOW_SIZE; y++)
				points.push_back(Point(x, y));
		// do cascade
		char filename[128];
		for(int i=0; i < atoi(argv[2]); ++i)
		{
			sprintf(filename, "stage%03d.txt", i);
			load_afeatures(afeatures, filename);
			verify(afeatures, iimg, points);
		}
		// show results
		if(points.size())
		{
			printf("found %d objects\n", points.size());
			for(size_t i=0; i<points.size(); i++)
			{
				Point pt1, pt2;
				Scalar scalar(255, 255, 0, 0);
				double width_scale = (double)orig.cols / (double)img_width;
				double height_scale = (double)orig.rows / (double)img_height;
				pt1.x = points[i].x * width_scale;
				pt1.y = points[i].y * height_scale;
				pt2.x = pt1.x + SUBWINDOW_SIZE * width_scale;
				pt2.y = pt1.y + SUBWINDOW_SIZE * height_scale;
				rectangle(orig, pt1, pt2, scalar);
			}
		}
		img_width *= 0.85;
		img_height *= 0.85;
	}
	imshow("Image", orig);
	waitKey();
	
	return 0;
}

