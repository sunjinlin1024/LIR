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

struct LIR_DLL LPQ_HEADER
{
	UINT head = LPQHEAD;
	UINT version = LPQCURRENT_VERSION;
	UINT fileCount;
	UINT contentSize;
	UINT hashOffset;
	UINT blockOffset;
	UINT emptyCount;
	UINT emptyOffset;
	UINT extra[LPQ_HEADER_SIZE / sizeof(UINT)-8];
};



typedef struct LIR_DLL LPQ_HASH_BLOCK
{
	UINT blockIndex;
}*LP_LPQ_HASH_TABLE;

typedef struct LIR_DLL LPQ_OFFSET_BLOCK
{
	
	UINT nHash;
	UINT nHashA;
	UINT nHashB;
	UINT fileOffset;
	UINT fileSize;
}*LP_LPQ_OFFSET_TABLE;

typedef struct LIR_DLL LPQ_EMPTY_BLOCK
{
	UINT offset;
	UINT size;
}*LP_LPQ_EMPTY_TABLE;

class LIR_DLL LPQFile :public FileHandler
{
public:
	//LPQFile(const char* mode);
	LPQFile();
	virtual ~LPQFile();
	FileStatus openLPQ(const std::string& fullPath, const char* mode);
	FileStatus exists(const std::string& fileName);
	FileStatus read(const std::string& fileName, Buffer* buffer);
	FileStatus write(const std::string& fileName, void* buff, size_t size);
	FileStatus create(const std::string& fullPath, int version = 1);

	
	FileStatus append(const std::string& fileName, void* buff, size_t size,const int index=-1);
	FileStatus flush();
	FileStatus resize(const int count);

	inline const int getCount()const{ return _header.fileCount; }

	//FileStatus pack(const std::string& fullPath, const std::string& dirRoot);
	//FileStatus unpack(const std::string& fullPach, const std::string& dirRoot);

	void reset();
protected:
	LPQ_HEADER _header;
	LP_LPQ_HASH_TABLE _hashTable;
	LP_LPQ_OFFSET_TABLE _blockTable;
	LP_LPQ_EMPTY_TABLE _emptyTable;
	LPFILE _file;
	
};

NS_LIR_END

#endif    // __LIR_FILEREADER_H__
