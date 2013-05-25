#include "adaboost.h"
#include <cstdio>

extern
void load_afeatures(vector<AdaBoostFeature*>& first_set, const char *file);
extern
bool is_object(const vector<AdaBoostFeature*> &afeatures, const Mat &iimg, int x, int y);

// count number of negative result
size_t neg_count(const vector<AdaBoostFeature*> &afeatures, const vector<Mat> &iis)
{
	size_t neg_cnt = 0;
	for(size_t i = 0; i < iis.size(); ++i)
	{
		if(!is_object(afeatures, iis[i], 0, 0))
			++neg_cnt;
	}
	return neg_cnt;
}

void build_cascade(unsigned int n_pos, unsigned int n_neg, const char *file)
{
	vector<Mat> pos_iis, neg_iis;
	vector<AdaBoostFeature*> afeatures;
	// load all features
	load_afeatures(afeatures, file);
	// load positive and negative set
	size_t num_skipped;
	num_skipped = LoadImage(n_pos, n_neg, pos_iis, neg_iis);
	printf("%d positive, %d negative, %d skipped\n", pos_iis.size(), neg_iis.size(), num_skipped);
	// calculate false rate
	double false_neg = neg_count(afeatures, pos_iis);
	double false_pos = neg_iis.size() - neg_count(afeatures, neg_iis);
	printf("false neg rate is %lf\n", false_neg / pos_iis.size());
	printf("false pos rate is %lf\n", false_pos / neg_iis.size());
}

int main(int argc, const char *argv[])
{
	if(argc < 4)
	{
		printf("\nUsage: %s n_positive n_negative "
		       "features_file\n\n", argv[0]);
		return 1;
	}
	unsigned int n_pos = atoi(argv[1]);
	unsigned int n_neg = atoi(argv[2]);
	build_cascade(n_pos, n_neg, argv[3]);
	return 0;
}

