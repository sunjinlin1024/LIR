#ifndef __LIR_BaseFile_H__
#define __LIR_BaseFile_H__

#include <string>

#include "platform/PlatformMacros.h"
#include "base/Buffer.h"

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
	Downloading,//Downloading
};

class LIR_DLL BaseFile{
public:
	//BaseFile(T1 t);//Constructor
	virtual FileStatus open(const std::string& fullPath, const char* mode)=0;
	virtual FileStatus close(){ return FileStatus::Success; };
	virtual FileStatus seek(const long& offset, const int& origin)=0;
	virtual FileStatus write(const void* buff, const size_t& size, const size_t& count)=0;
	virtual FileStatus read(void* buff, const size_t& size, const size_t& count)=0;
	virtual void getSize(size_t& size)=0;
	virtual ~BaseFile();
protected:
	BaseFile();
	
};

class LIR_DLL SingleFileC:public BaseFile
{
public:
	SingleFileC();
	virtual ~SingleFileC();
	virtual FileStatus open(const std::string& fullPath, const char* mode) override;
	virtual FileStatus close() override;
	virtual FileStatus seek(const long& offset, const int& origin) override;
	virtual FileStatus write(const void* buff, const size_t& size, const size_t& count) override;
	virtual FileStatus read(void* buff, const size_t& size, const size_t& count) override;
	virtual void getSize(size_t& size) override;
	virtual FileStatus read(Buffer* buffer);
protected:
	FILE* _file;
};

NS_LIR_END

#endif    // __LIR_BaseFile_H__
