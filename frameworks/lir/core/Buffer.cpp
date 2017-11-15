#include "core/Buffer.h"
#include <stdlib.h>

#include "core/Macros.h"

USING_NS_LIR


Buffer::Buffer():
_size(0), 
_buff(nullptr)
{
}

Buffer::~Buffer()
{
	clear();
}

void Buffer :: clear()
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
		_buff=realloc(_buff, size);
	}
	_size = size;
}

void Buffer::fastSet(void* buff, size_t size)
{
	_buff = buff;
	_size = size;
}



