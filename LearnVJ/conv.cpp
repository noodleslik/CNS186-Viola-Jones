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
	
	unsigned int i, type, x1, y1, x2, y2;// UL, LR points of rectangle1
	double threshold, polarity, beta_t;
	i = 1;
	while(!fin.eof())
	{
		fin>>type>>x1>>y1>>x2>>y2;
		fin>>threshold>>polarity>>beta_t;
		cout<<type<<"["<<x1<<", "<<y1<<"]["<<x2<<", "<<y2<<"] ";
		cout<<threshold<<" "<<polarity<<" "<<beta_t<<endl;
		unsigned int w = x2 - x1;
		unsigned int h = y2 - y1;
		fout<<i<<endl;
		if(type == TWO_REC_HORIZ)
		{
			fout<<x1<<" "<<y1<<" "<<w<<" "<<h<<" -1"<<endl;
			fout<<x1+w<<" "<<y1<<" "<<w<<" "<<h<<" 1"<<endl;
		}
		else if(type == TWO_REC_VERT)
		{
			fout<<x1<<" "<<y1<<" "<<w<<" "<<h<<" -1"<<endl;
			fout<<x1<<" "<<y1+h<<" "<<w<<" "<<h<<" 1"<<endl;
		}
		else if(type == THREE_REC_HORIZ)
		{
			fout<<x1<<" "<<y1<<" "<<3*w<<" "<<h<<" -1"<<endl;
			fout<<x1+w<<" "<<y1<<" "<<w<<" "<<h<<" 2"<<endl;
		}
		else if(type == THREE_REC_VERT)
		{
			fout<<x1<<" "<<y1<<" "<<w<<" "<<3*h<<" -1"<<endl;
			fout<<x1<<" "<<y1+h<<" "<<w<<" "<<h<<" 2"<<endl;
		}
		else if(type == FOUR_REC)
		{
			fout<<x1<<" "<<y1<<" "<<2*w<<" "<<2*h<<" -1"<<endl;
			fout<<x1<<" "<<y1<<" "<<w<<" "<<h<<" 2"<<endl;
			fout<<x1+w<<" "<<y1+h<<" "<<w<<" "<<h<<" 2"<<endl;
		}
		else
		{
			cout<<"Type error"<<endl;
			return -1;
		}
		fout<<threshold<<" "<<polarity<<" "<<beta_t<<endl;
		i++;
	}
	fin.close();
	fout.close();
	return 0;
}

