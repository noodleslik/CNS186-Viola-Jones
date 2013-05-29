#include "adaboost.h"
#include <ctime>
#include <cstdio>
#include <cstdlib>
using namespace std;

int main(int argc, const char* argv[])
{
	vector<AdaBoostFeature*> afeatures;
	const char* const filename = "billabingbong.txt";
	if(argc < 5)
	{
		printf("\nUsage: %s n_positive n_negative "
		       "how_many_feature n_random_feature\n\n", argv[0]);
		return 1;
	}
	unsigned int n_pos = atoi(argv[1]);
	unsigned int n_neg = atoi(argv[2]);
	unsigned int how_many = atoi(argv[3]);
	unsigned int n_random = atoi(argv[4]);
	clock_t start = clock();
	// Run adaboost training
	afeatures = RunAdaBoost(n_pos, n_neg, how_many, n_random);
	double minutes = double(clock()-start)/CLOCKS_PER_SEC/60;
	printf("\nTrain used %lf minutes\n\n", minutes);
	SaveAdaBoost(afeatures, filename);
	return 0;
}

