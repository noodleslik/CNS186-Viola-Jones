#include "adaboost.h"
#include <cstdio>

extern
void load_afeatures(vector<AdaBoostFeature*>& first_set, const char *file);
extern
bool is_object(const vector<AdaBoostFeature*> &afeatures, const Mat &iimg, int x, int y);

// count number of passed result
double pos_count(const vector<AdaBoostFeature*> &afeatures, const array<Mat> &iis)
{
	double pos_cnt = 0;
	for(size_t i = 0; i < iis.size(); ++i)
	{
		if(is_object(afeatures, iis[i], 0, 0))
			++pos_cnt;
	}
	return pos_cnt;
}

void build_cascade(unsigned int n_pos, unsigned int n_neg, const char *file)
{
	array<Mat> pos_iis;
	array<Mat> neg_iis;
	vector<AdaBoostFeature*> afeatures;
	// load all features
	load_afeatures(afeatures, file);
	// load positive and negative set
	size_t num_skipped;
	num_skipped = LoadImage(n_pos, n_neg, pos_iis, neg_iis);
	printf("%d positive, %d negative, %d skipped\n", pos_iis.size(), neg_iis.size(), num_skipped);
	// calculate false rate
	char filename[128];
	size_t feature_i = 0, stage_i = 0;
	double detect_rate, false_pos_rate;
	#define False_pos          0.12
	#define False_pos_update   (1 - False_pos)
	double False_pos_rate = False_pos;
	vector<AdaBoostFeature*> features;
	while(feature_i < afeatures.size())
	{
		features.push_back(afeatures[feature_i]);
		detect_rate = pos_count(features, pos_iis) / pos_iis.size();
		false_pos_rate = pos_count(features, neg_iis) / neg_iis.size();
		//printf("[%lf - %lf]", detect_rate, false_pos_rate);
		if(false_pos_rate < False_pos_rate)
		{
			sprintf(filename, "stage%03d.txt", stage_i);
			printf("**detection rate %lf, false positive %lf(%lf)\n",
					detect_rate, false_pos_rate, False_pos_rate);
			printf("**save %s %d features\n", filename, features.size());
			SaveAdaBoost(features, filename);
			features.clear();
			++stage_i;
			False_pos_rate *= False_pos_update;
		}
		++feature_i;
	}
	printf("**left %d features\n", features.size());
	sprintf(filename, "stage%03d.txt", stage_i);
	printf("**save %s %d features\n", filename, features.size());
	SaveAdaBoost(features, filename);
	
	printf("**Total %d stage cascades\n", stage_i);
}

int main(int argc, const char *argv[])
{
	if(argc < 4)
	{
		printf("\nUsage: %s n_positive n_negative features_file\n\n", argv[0]);
		return 1;
	}
	unsigned int n_pos = atoi(argv[1]);
	unsigned int n_neg = atoi(argv[2]);
	build_cascade(n_pos, n_neg, argv[3]);
	return 0;
}

