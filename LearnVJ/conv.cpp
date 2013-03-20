#include <iostream>
#include <fstream>
#include <string>
#include "features.h"

using namespace std;

/**
 * Convert train result's format
 */
int main()
{
	ifstream fin;
	fin.open("billabingbong.txt");
	ofstream fout;
	fout.open("trees.txt");
	
	string line;
	unsigned int type, x1, y1, x2, y2;
	double threshold, polarity, beta_t;
	while(!fin.eof())
	{
		fin>>type>>x1>>y1>>x2>>y2;
		fin>>threshold>>polarity>>beta_t;
	}
	return 0;
}

