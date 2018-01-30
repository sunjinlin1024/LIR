#include "platform/PlatformConfig.h"
#if LIR_TARGET_PLATFORM == LIR_PLATFORM_WIN32

#include "platform/StdC.h"

#ifndef __MINGW32__

NS_LIR_BEGIN
int gettimeofday(struct timeval* val, struct timezone *)
{
	if (val)
	{
		LARGE_INTEGER liTime, liFreq;
		QueryPerformanceFrequency(&liFreq);
		QueryPerformanceCounter(&liTime);
		val->tv_sec = (long)(liTime.QuadPart / liFreq.QuadPart);
		val->tv_usec = (long)(liTime.QuadPart * 1000000.0 / liFreq.QuadPart - val->tv_sec * 1000000.0);
	}
	return 0;
}

NS_LIR_END

#endif // __MINGW32__

#endif // LIR_TARGET_PLATFORM == LIR_PLATFORM_WIN32
