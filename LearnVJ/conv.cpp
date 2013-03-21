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
	unsigned int i, type, x1, y1, x2, y2;
	double threshold, polarity, beta_t;
	i = 1;
	while(!fin.eof())
	{
		fin>>type>>x1>>y1>>x2>>y2;
		fin>>threshold>>polarity>>beta_t;
		cout<<type<<"["<<x1<<", "<<y1<<"]["<<x2<<", "<<y2<<"] ";
		cout<<threshold<<" "<<polarity<<" "<<beta_t<<endl;
		if(type == TWO_REC_HORIZ)
		{}
		else if(type == TWO_REC_VERT)
		{}
		else if(type == THREE_REC_HORIZ)
		{}
		else if(type == THREE_REC_VERT)
		{}
		else if(type == FOUR_REC)
		{}
		else
		{
			cout<<"Type error"<<endl;
			return -1;
		}
		
		i++;
	}
	fin.close();
	fout.close();
	return 0;
}

