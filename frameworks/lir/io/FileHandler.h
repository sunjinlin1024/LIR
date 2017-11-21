#ifndef __LIR_FILEHANDLER_H__
#define __LIR_FILEHANDLER_H__

#include <string>
#include <unordered_map>

#include "platform/PlatformMacros.h"
#include "core/Buffer.h"

NS_LIR_BEGIN

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

class LIR_DLL FileHandler
{
public:
	FileStatus open(const std::string& fullPath,const char* mode, FILE* &file, size_t& size);
};

class LIR_DLL FileHandlerSingle :public FileHandler
{
public:
	FileHandlerSingle();
	~FileHandlerSingle();
	virtual FileStatus read(const std::string& fullPath, Buffer* buffer);
	virtual FileStatus write(const std::string& fullPath, void* buff, size_t size);
protected:
	//FILE *_file;
};

NS_LIR_END

#endif    // __LIR_FILEHANDLER_H__
