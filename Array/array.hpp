#ifndef __ARRAY_HPP__
#define __ARRAY_HPP__

#include <cstddef>
#include <cstdlib>
#include <cstring>
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
		ptr = (T*)malloc(sizeof(T)*_INI_ARRAY_SIZE);
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
	void push_back(T ele)
	{
		if(index > _size)
			throw;
		if(index == _size)
		{
			T *temp = (T*)malloc(sizeof(T)*_size*2);
			memcpy(temp, ptr, sizeof(T)*_size);
			_size *= 2;
			free(ptr);
			ptr = temp;
		}
		*(ptr + index) = ele;
		++index;
	}
	~array()
	{
		free(ptr);
	}
};

#endif

