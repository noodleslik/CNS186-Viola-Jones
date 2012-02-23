#include "adaboost.h"

using namespace std;

int main() {
    const char* const filename1 = "billabingbong.txt";
    vector<AdaBoostFeature*> ten_of_fiddy = RunAdaBoost(2056, 2056, 10, 50);
    SaveAdaBoost(ten_of_fiddy, filename1); 
}
