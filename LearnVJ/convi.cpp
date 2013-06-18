#include <iostream>
#include <fstream>
#include <string>
#include "features.h"

using namespace std;

/**
 * Convert train result's from floatint point to integral
 */
int main(int argc, char *argv[])
{
	if(argc < 2)
		return 1;
	ifstream fin;
	fin.open(argv[1]);
	ofstream fout;
	fout.open("treesi.txt");
	
	unsigned int t, trees;
	unsigned int type, x1, y1, x2, y2;// UL, LR points of rectangle1
	int polarity, size;
	double threshold, beta_t, false_pos_rate;
	fin >> size >> trees;
	// misc at begin of file
	fout << size << endl << trees << endl;
	double max_threshold = 0, max_ialpha = 0;
	// process each weak classifier
	t = 0;
	while(!fin.eof() && t<trees)
	{
		unsigned int ialpha;
		// read in
		fin >> type >> x1 >> y1 >> x2 >> y2;
		fin >> threshold >> polarity >> beta_t >> false_pos_rate;
		if(threshold > max_threshold)
			max_threshold = threshold;

		double alpha = -log(beta_t);
		ialpha = (unsigned int)(alpha * 100);

		if(alpha - ialpha/100.0 > 0.005)
			++ialpha;
		if(ialpha > max_ialpha)
			max_ialpha = ialpha;

		// write out
		// Attention! store alpha value instead of beta_t for HW implementation
		fout << type << " " << x1 << " " << y1 << " " << x2 << " " << y2 << " "
			 << threshold << " " << polarity << "  ";
		fout << ialpha << " " << false_pos_rate << endl;

		++t;
	}

	cout << "max threshold " << max_threshold << endl;
	cout << "max ialpha " << max_ialpha << endl;
	fin.close();
	fout.close();
	return 0;
}

