#include "adaboost.h"
#include <cstdio>

extern
void load_afeatures(vector<AdaBoostFeature*>& first_set, const char *file);
extern
void verify(const vector<AdaBoostFeature*> &features, const char *imfile);

// verify a single stage

int main(int argc, char *argv[])
{
	if(argc < 3)
	{
		printf("Usage: %s image cascade_file\n", argv[0]);
		return 1;
	}
	vector<AdaBoostFeature*> first_set;
	load_afeatures(first_set, argv[2]);
	verify(first_set, argv[1]);
	return 0;
}

