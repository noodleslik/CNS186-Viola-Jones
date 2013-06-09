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
	fout.open("trees.txt");
	
	unsigned int t, trees;
	unsigned int type, x1, y1, x2, y2;// UL, LR points of rectangle1
	int polarity, size;
	double threshold, beta_t, false_pos_rate;
	fin >> size >> trees;
	// misc at begin of file
	fout << size << endl << trees << endl;

	// process each weak classifier
	t = 0;
	while(!fin.eof() && t<trees)
	{
		unsigned int ialpha;
		// read in
		fin >> type >> x1 >> y1 >> x2 >> y2;
		fin >> threshold >> polarity >> beta_t >> false_pos_rate;

		double alpha = -log(beta_t);
		ialpha = (unsigned int)(alpha * 10);

		if(alpha - ialpha/10.0 > 0.05)
			++ialpha;

		// write out
		// Attention! store alpha value instead of beta_t for HW implementation
		fout << type << " " << x1 << " " << y1 << " " << x2 << " " << y2 << " "
			 << threshold << " " << polarity << "  ";
		fout << ialpha << " " << false_pos_rate << endl;

		++t;
	}

	fin.close();
	fout.close();
	return 0;
}

