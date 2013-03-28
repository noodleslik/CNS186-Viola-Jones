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
	
	unsigned int s, t, trees;
	unsigned int type, x1, y1, x2, y2;// UL, LR points of rectangle1
	int polarity, size;
	double threshold, beta_t, stage_threshold;
	s = 1; t = 1;
	stage_threshold = 0;
	fin>>size>>trees;
	// misc at begin of file
	fout<<size<<" "<<size<<endl<<"1\n"<<trees<<endl;
	// Assume we have only 1 stage
	while(!fin.eof() && t<=trees)
	{
		fin>>type>>x1>>y1>>x2>>y2;
		fin>>threshold>>polarity>>beta_t;
		cout<<type<<"["<<x1<<", "<<y1<<"]["<<x2<<", "<<y2<<"] ";
		cout<<threshold<<" "<<polarity<<" "<<beta_t<<endl;
		// two or three rects
		unsigned int w = x2 - x1;
		unsigned int h = y2 - y1;
		if(type == TWO_REC_HORIZ)
		{
			fout<<s<<" "<<t<<" "<<1<<" ";
			fout<<"1 "<<x1<<" "<<y1<<" "<<w<<" "<<h<<" 1"<<endl;
			fout<<s<<" "<<t<<" "<<1<<" ";
			fout<<"2 "<<x2<<" "<<y1<<" "<<w<<" "<<h<<" -1"<<endl;
		}
		else if(type == TWO_REC_VERT)
		{
			fout<<s<<" "<<t<<" "<<1<<" ";
			fout<<"1 "<<x1<<" "<<y1<<" "<<w<<" "<<h<<" 1"<<endl;
			fout<<s<<" "<<t<<" "<<1<<" ";
			fout<<"2 "<<x1<<" "<<y2<<" "<<w<<" "<<h<<" -1"<<endl;
		}
		else if(type == THREE_REC_HORIZ)
		{
			fout<<s<<" "<<t<<" "<<1<<" ";
			fout<<"1 "<<x1<<" "<<y1<<" "<<3*w<<" "<<h<<" 1"<<endl;
			fout<<s<<" "<<t<<" "<<1<<" ";
			fout<<"2 "<<x2<<" "<<y1<<" "<<w<<" "<<h<<" -2"<<endl;
		}
		else if(type == THREE_REC_VERT)
		{
			fout<<s<<" "<<t<<" "<<1<<" ";
			fout<<"1 "<<x1<<" "<<y1<<" "<<w<<" "<<3*h<<" 1"<<endl;
			fout<<s<<" "<<t<<" "<<1<<" ";
			fout<<"2 "<<x1<<" "<<y2<<" "<<w<<" "<<h<<" -2"<<endl;
		}
		else if(type == FOUR_REC)
		{
			fout<<s<<" "<<t<<" "<<1<<" ";
			fout<<"1 "<<x1<<" "<<y1<<" "<<2*w<<" "<<2*h<<" 1"<<endl;
			fout<<s<<" "<<t<<" "<<1<<" ";
			fout<<"2 "<<x2<<" "<<y1<<" "<<w<<" "<<h<<" -2"<<endl;
			fout<<s<<" "<<t<<" "<<1<<" ";
			fout<<"3 "<<x1<<" "<<y2<<" "<<w<<" "<<h<<" -2"<<endl;
		}
		else
		{
			cout<<"Type error"<<endl;
			return -1;
		}
		// titled
		fout<<s<<" "<<t<<" "<<1<<" "<<0<<endl;
		// threshold
		fout<<s<<" "<<t<<" "<<1<<" "<<threshold*polarity/size/size<<endl;
		// left and right value
		double alpha = log(1./beta_t);
		//cout << beta_t << "  " << alpha <<"\n";
		fout<<s<<" "<<t<<" "<<1<<" "<<alpha<<endl; //left
		fout<<s<<" "<<t<<" "<<1<<" "<<"0.0"<<endl; //right
		stage_threshold += alpha;
		t++;
	}
	// stage threshold
	fout<<s<<" "<< stage_threshold / 2 <<endl;
	fin.close();
	fout.close();
	return 0;
}

