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

	void fastSet(void* buff, size_t size);

	inline void* buffer()const{
		return _buff;
	};

	inline const size_t size()const{
		return _size;
	};
private:
	size_t _size;
	void* _buff;
};


NS_LIR_END

#endif    // __LIR_BUFFER_H__
