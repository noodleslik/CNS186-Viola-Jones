#include "adaboost.h"
#include "features.h"
#include "../IntegralImage/integral_image.h"
#include <cstdio>
#include <fstream>
using namespace cv;
using namespace std;

//#define STORE_ALPHA

bool is_object(const vector<AdaBoostFeature*> &afeatures, const Mat &iimg, int x, int y)
{
	double stage_sum = 0;
	double stage_threshold = 0;
	for(size_t fi = 0; fi < afeatures.size(); fi++)
	{
		double alpha;
		Feature feature;
		int feature_val, polarity, threshold;
#ifndef STORE_ALPHA
		alpha = -log(afeatures[fi]->beta_t);
#else
		alpha = afeatures[fi]->beta_t;
#endif
		polarity = afeatures[fi]->polarity;
		threshold = afeatures[fi]->threshold;
		feature.type = afeatures[fi]->feature->type;
		feature.x1 = afeatures[fi]->feature->x1 + x;
		feature.y1 = afeatures[fi]->feature->y1 + y;
		feature.x2 = afeatures[fi]->feature->x2 + x;
		feature.y2 = afeatures[fi]->feature->y2 + y;
		feature_val = CalculateFeature(&feature, iimg);
		if(feature_val * polarity < threshold * polarity)
			stage_sum += alpha;
		stage_threshold += alpha;
	}
	if(stage_sum > 0.5*stage_threshold)// pass the stage
		return true;
	else
		return false;
}

void verify(const vector<AdaBoostFeature*> &afeatures, const Mat &iimg, vector<Point> &pnts)
{
	vector<Point> points;
	for(size_t i = 0; i < pnts.size(); ++i)
		if(is_object(afeatures, iimg, pnts[i].x, pnts[i].y))
			points.push_back(pnts[i]);
	pnts = points;
}

void verify(const vector<AdaBoostFeature*> &afeatures, const char * imfile)
{
	Mat orig;
	orig = imread(imfile, 0);
	int img_width = orig.cols;
	int img_height = orig.rows;
	while(img_width > 3*SUBWINDOW_SIZE && img_height > 3*SUBWINDOW_SIZE)
	{
		int x, y, found;
		Mat img, iimg;
		resize(orig, img, Size(img_width, img_height));
		iimg = IntegralImage(img);
		found = 0;
		for(x = 0; x < img_width - SUBWINDOW_SIZE; x++)
		{
			for(y = 0; y < img_height - SUBWINDOW_SIZE; y++)
			{
				if(is_object(afeatures, iimg, x, y))
				{
					Point pt1, pt2;
					Scalar scalar(255, 255, 0, 0);
					found++;
					printf("(%d,%d)", x, y);
					pt1.x = x; pt2.x = x + SUBWINDOW_SIZE;
					pt1.y = y; pt2.y = y + SUBWINDOW_SIZE;
					rectangle(img, pt1, pt2, scalar);
				}
			}
		}
		if(found)
		{
			printf("\nFound %d at %d x %d\n", found, img_width, img_height);
			imshow("Image", img);
			waitKey();
		}
		img_width *= 0.8;
		img_height *= 0.8;
	}
}

void load_afeatures(vector<AdaBoostFeature*>& first_set, const char *file)
{
	AdaBoostFeature * afeature;
	int size, count, num;
	ifstream fin;
	fin.open(file);
	if(!fin.is_open())
	{
		printf("load_afeatures(): %s open error!\n", file);
		return;
	}
	fin >> size >> count;
	printf("size %d, ", size);
	num = 0;
	first_set.clear();
	while(!fin.eof() && num < count)
	{
		int type;
		afeature = new AdaBoostFeature;
		afeature->feature = new Feature;
		fin >> type;
		afeature->feature->type = (FeatureTypeT)type;
		fin >> afeature->feature->x1 >> afeature->feature->y1
			>> afeature->feature->x2 >> afeature->feature->y2
			>> afeature->threshold >> afeature->polarity
			>> afeature->beta_t >> afeature->false_pos_rate;
		first_set.push_back(afeature);
		num++;
	}
	printf("%d features loaded\n", num);
	fin.close();
}

