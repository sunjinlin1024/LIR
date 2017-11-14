#ifndef __LIR_BUFFER_H__
#define __LIR_BUFFER_H__

#include "platform/PlatformMacros.h"

NS_LIR_BEGIN

class LIR_DLL Buffer
{
public:
	Buffer();
	~Buffer();

	void clear();

	void resize(size_t size);

	void* buffer();

	size_t size();
private:
	size_t _size;
	void* _buff;
};


NS_LIR_END

#endif    // __LIR_BUFFER_H__
