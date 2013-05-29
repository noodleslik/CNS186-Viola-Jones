#ifndef __ARRAY_HPP__
#define __ARRAY_HPP__

#include <cstddef>
using namespace std;

#define _INI_ARRAY_SIZE 4096

template <class T>
class array
{
private:
	T * ptr;
	size_t _size;
	size_t index;
public:
	explicit array()
	{
		index = 0;
		_size = _INI_ARRAY_SIZE;
		ptr = new T[_size];
	}
	inline size_t size() const
	{
		return index;
	}
	bool empty() const
	{
		return (index == 0);
	}
	T operator [] (size_t idx) const
	{
		if(idx < _size)
			return *(ptr+idx);
		else
			throw;
	}
	T& operator [] (size_t idx)
	{
		if(idx < _size)
			return *(ptr+idx);
		else
			throw;
	}
	//Warning! this function isn't thread safe
	void push_back(const T& ele)
	{
		if(index > _size)
			throw;
		if(index == _size)
		{
			T *temp = new T[_size*2];
			for(size_t i = 0; i < index; ++i)
			{
				temp[i] = ptr[i];
			}
			_size *= 2;
			delete [] ptr;
			ptr = temp;
		}
		*(ptr + index) = ele;
		++index;
	}
	~array()
	{
		delete [] ptr;
	}
};

#endif

