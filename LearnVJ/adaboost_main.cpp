#include "adaboost.h"
#include <cstdio>
#include <cstdlib>
using namespace std;

int main(int argc, const char* argv[])
{
	vector<AdaBoostFeature*> first_set;
	const char* const filename1 = "billabingbong.txt";
	if(argc < 5)
	{
		printf("\nUsage: %s n_positive n_negative "
		       "how_many_feature n_random_feature\n\n", argv[0]);
		return 1;
	}
	unsigned int n_pos = atoi(argv[1]); // 1000
	unsigned int n_neg = atoi(argv[2]); // 2000
	unsigned int how_many = atoi(argv[3]); // 20
	unsigned int n_random = atoi(argv[4]); // 5000
	first_set = RunAdaBoost(n_pos, n_neg, how_many, n_random);
	SaveAdaBoost(first_set, filename1);
	return 0;
}

