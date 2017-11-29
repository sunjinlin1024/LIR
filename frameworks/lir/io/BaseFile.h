#ifndef __LIR_BaseFile_H__
#define __LIR_BaseFile_H__

#include <string>
#include <unordered_map>

#include "platform/PlatformMacros.h"
#include "core/Buffer.h"

NS_LIR_BEGIN

#if LIR_TARGET_PLATFORM==LIR_PLATFORM_WIN32
#define LFILE FILE
#define LPFILE FILE*
#elif LIR_TARGET_PLATFORM == LIR_PLATFORM_ANDROID
#define  LFILE AAsset 
#define  LPFILE AAsset*
#endif

enum LIR_DLL FileStatus
{
	Success = 0,
	NotExists, // File not exists
	OpenFailed, // Open file failed.
	ReadFailed, // Read failed
	WriteFailed, // Read failed
	SeekFailed, // Seek failed
	NotInitialized, // FileUtils is not initializes
	TooLarge, // The file is too large (great than 2^32-1)
	ObtainSizeFailed, // Failed to obtain the file size.
	Openend, //  file Opened.
	Downloading,//Downloading
};

class LIR_DLL BaseFile
{
public:
	FileStatus fopen(const std::string& fullPath, const char* mode, LPFILE &file, size_t& size);
protected:
	int fclose(LPFILE file);
	int fseek(LPFILE file,long _Offset, int _Origin);
	int fwrite(const void* buff, size_t size, int count, LPFILE file);
	int fread(void* buff, size_t size, int count, LPFILE file);
};

class LIR_DLL SingleFile :public BaseFile
{
public:
	SingleFile();
	~SingleFile();
	virtual FileStatus read(const std::string& fullPath, Buffer* &buffer);
protected:
	//FILE *_file;
};

NS_LIR_END

#endif    // __LIR_BaseFile_H__
