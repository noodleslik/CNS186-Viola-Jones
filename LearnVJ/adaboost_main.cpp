#include "adaboost.h"

using namespace std;

int main() {
    const char* const filename1 = "billabingbong.txt";
    vector<AdaBoostFeature*> first_set = RunAdaBoost(2056, 2056, 20, 5000);
    SaveAdaBoost(first_set, filename1); 
}
