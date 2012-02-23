#include "adaboost.h"

using namespace std;
using namespace cv;

int main() {
    vector<AdaBoostFeature*> 10_of_50 = RunAdaBoost(2056, 2056, 10, 50);
    SaveAdaBoost(10_of_50, "billabingbong.txt"); 
}
