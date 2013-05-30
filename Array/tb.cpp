#include "array.hpp"

#include <cmath>
#include <iostream>
using namespace std;

#define GEN(i) ((i*2. + 1.)/3.)

int main()
{
	array<double> a1;
	for(size_t i = 0; i < _INI_ARRAY_SIZE*100; i++)
	{
		a1.push_back( GEN(i) );
	}
	for(size_t i = 0; i < a1.size(); i++)
	{
		if( fabs(a1[i] - GEN(i)) >= 1e-6)
		{
			cout<<i<<": "<<a1[i]<<" != "<<GEN(i)<<endl;
			return -1;
		}
	}
	array<double> a2;
	for(size_t i = 1; i < 10; i++)
	{
		a2.push_back( i*10 );
	}
	a2.erase(8);
	a2.erase(0);
	for(size_t i = 0; i < a2.size(); i++)
	{
		cout << a2[i] << " ";
	}
	cout<<endl;
	return 0;
}

