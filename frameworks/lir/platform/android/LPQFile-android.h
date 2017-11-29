#ifndef __LIR_LPQFILE_ANDROID_H__
#define __LIR_LPQFILE_ANDROID_H__

#include "platform/PlatformMacros.h"

#if LIR_TARGET_PLATFORM==LIR_PLATFORM_ANDROID

#include "core/Buffer.h"
#include "io/LPQFile.h"

class LIR_DLL LPQFileAndroid :public LPQFile
{
public:
	//LPQFileAndroid(const char* mode);
	LPQFileAndroid();
	virtual ~LPQFileAndroid();
	FileStatus openLPQ(const std::string& fullPath, const char* mode) override;
	FileStatus exists(const std::string& fileName)override;
	FileStatus read(const std::string& fileName, Buffer* buffer)override;
	FileStatus write(const std::string& fileName, void* buff, size_t size)override;
	FileStatus create(const std::string& fullPath, int version = 1)override;
};

#endif

#endif    // __LIR_LPQFILE_ANDROID_H__
