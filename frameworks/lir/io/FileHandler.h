#ifndef __LIR_FILEREADER_H__
#define __LIR_FILEREADER_H__

#include <string>
#include <unordered_map>

#include "platform/PlatformMacros.h"
#include "core/Buffer.h"

NS_LIR_BEGIN


#ifndef UCHAR
typedef unsigned char UCHAR;
#endif

#ifndef UINT 
typedef unsigned int UINT;
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


#define MPQ_HEADER 0xABCC
#define MPQ_CURRENT_VERSION 1

const int MPQ_HEADER_SIZE=208;

typedef struct MPQHEADER
{
	UINT head = MPQ_HEADER;
	UINT version = MPQ_CURRENT_VERSION;
	UINT fileCount;
	UINT contentSize;
	UINT hashOffset;
	UINT blockOffset;
	char extra[MPQ_HEADER_SIZE-sizeof(UINT)*6];
};



typedef struct MPQHASHTABLE
{
	UINT blockIndex;
}*LPMPQHASHTABLE;

typedef struct MPQBLOCKTABLE
{
	
	UINT nHash;
	UINT nHashA;
	UINT nHashB;
	UINT fileOffset;
	UINT fileSize;
}*LPLPMPQHASHTABLE;


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


class LIR_DLL FileHandlerPack :public FileHandler
{
public:
	//FileHandlerPack(const char* mode);
	FileHandlerPack();
	virtual ~FileHandlerPack();
	FileStatus openByPack(const std::string& fullPath, const char* mode);
	//FileStatus exists(const std::string& fileName);
	FileStatus read(const std::string& fileName, Buffer* buffer);
	FileStatus write(const std::string& fullPath,const std::string& fileName, void* buff, size_t size);
	FileStatus create(const std::string& fullPath, int version = 1);

	
	FileStatus append(const std::string& fullPath, const std::string& fileName, void* buff, size_t size,const int index=-1);
	FileStatus flush();
	FileStatus resize(const int count);

	inline const int getCount()const{ return _header.fileCount; }

	//FileStatus pack(const std::string& fullPath, const std::string& dirRoot);
	//FileStatus unpack(const std::string& fullPach, const std::string& dirRoot);

	void reset();
protected:
	MPQHEADER _header;
	MPQHASHTABLE* _hashTable;
	MPQBLOCKTABLE* _blockTable;
	FILE *_file;
	
};

NS_LIR_END

#endif    // __LIR_FILEREADER_H__
