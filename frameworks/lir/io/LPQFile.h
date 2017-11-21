#ifndef __LIR_FILEREADER_H__
#define __LIR_FILEREADER_H__

#include <string>
#include <unordered_map>

#include "platform/PlatformMacros.h"
#include "core/Buffer.h"
#include "io/FileHandler.h"

NS_LIR_BEGIN


#ifndef UCHAR
typedef unsigned char UCHAR;
#endif

#ifndef UINT 
typedef unsigned int UINT;
#endif




#define LPQHEAD 0xABCC
#define LPQCURRENT_VERSION 1

const int LPQ_HEADER_SIZE = 208;

typedef struct LPQHEADER
{
	UINT head = LPQHEAD;
	UINT version = LPQCURRENT_VERSION;
	UINT fileCount;
	UINT contentSize;
	UINT hashOffset;
	UINT blockOffset;
	char extra[LPQ_HEADER_SIZE - sizeof(UINT)* 6];
};



typedef struct LPQHASHTABLE
{
	UINT blockIndex;
}*LPLPQHASHTABLE;

typedef struct LPQBLOCKTABLE
{
	
	UINT nHash;
	UINT nHashA;
	UINT nHashB;
	UINT fileOffset;
	UINT fileSize;
}*LPLPLPQHASHTABLE;


class LIR_DLL LPQFile :public FileHandler
{
public:
	//LPQFile(const char* mode);
	LPQFile();
	virtual ~LPQFile();
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
	LPQHEADER _header;
	LPQHASHTABLE* _hashTable;
	LPQBLOCKTABLE* _blockTable;
	FILE *_file;
	
};

NS_LIR_END

#endif    // __LIR_FILEREADER_H__
