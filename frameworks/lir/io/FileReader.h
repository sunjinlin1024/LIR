#ifndef __LIR_FILEREADER_H__
#define __LIR_FILEREADER_H__

#include <string>
#include <unordered_map>

#include "platform/PlatformMacros.h"

NS_LIR_BEGIN

enum FileStatus
{
	Success = 0,
	NotExists, // File not exists
	OpenFailed, // Open file failed.
	ReadFailed, // Read failed
	NotInitialized, // FileUtils is not initializes
	TooLarge, // The file is too large (great than 2^32-1)
	ObtainSizeFailed, // Failed to obtain the file size.
	Openend, //  file Opened.
	Downloading,//Downloading
};

typedef struct FilePackInfo{
	size_t offset;
	size_t size;
};

class LIR_DLL Buffer
{
public:
	Buffer() :_size(0), _buff(nullptr){};
	~Buffer(){
		LIR_SAFE_FREE(_buff);
		_size = 0;

	};

	void resize(size_t size){
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

	unsigned char* buffer(){
		return _buff;
	}

	size_t size()
	{
		return _size;
	}
private:
	size_t _size;
	unsigned char* _buff;
};


class LIR_DLL FileReader
{
public:
	virtual FileStatus read(const std::string& filename, Buffer* buffer)=0;
protected:
	FileReader();
	virtual ~FileReader();
	FileStatus openFile(const std::string& filename, FILE* file, size_t& size);
};

class LIR_DLL FileReaderSingle:FileReader
{
public:
	FileReaderSingle();
	~FileReaderSingle();
	virtual FileStatus read(const std::string& filename, Buffer* buffer) override;
protected:
	FILE *_file;
};


class LIR_DLL FileReaderPack :FileReader
{
public:
	FileReaderPack();
	~FileReaderPack();
	virtual FileStatus read(const std::string& filename, Buffer* buffer) override;
	FileStatus initByPackFile(const std::string& packname);
protected:
	size_t _contentOffset;
	FILE *_file;
	mutable std::unordered_map<std::string,FilePackInfo> _fileDic;
};



NS_LIR_END

#endif    // __LIR_FILEREADER_H__
