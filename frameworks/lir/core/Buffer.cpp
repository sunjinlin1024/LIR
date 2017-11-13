#include "core/Buffer.h"
#include <stdlib.h>

USING_NS_LIR


Buffer::Buffer():
_size(0), 
_buff(nullptr)
{
}

Buffer::~Buffer()
{
	LIR_SAFE_FREE(_buff);
	_size = 0;
}

void Buffer::resize(size_t size)
{
	if (size == _size)
	{
		return;
	}
	else if (size >_size)
	{
		realloc(_buff, size);
	}
	_size = size;
}

unsigned char* Buffer::buffer()
{
	return _buff;
}

size_t Buffer::size()
{
	return _size;
}



