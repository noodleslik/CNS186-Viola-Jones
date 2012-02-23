#include "adaboost.h"

using namespace std;
using namespace cv;

int main() {
    vector<AdaBoostFeature*> ten_of_fiddy = RunAdaBoost(2056, 2056, 10, 50);
    SaveAdaBoost(ten_of_fiddy, "billabingbong.txt"); 
}
