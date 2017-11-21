#include "io/LPQFile.h"

//#include <stack>
#include <sys/stat.h>

#include "debug/Debug.h"

USING_NS_LIR

#ifdef __cplusplus
extern "C" {
#endif


	unsigned long dwCryptTable[0x500];
	const int HASH_OFFSET = 0, HASH_A = 1, HASH_B = 2;

	void InitializeCryptTable()
	{
		unsigned long seed = 0x00100001;
		unsigned long index1 = 0;
		unsigned long index2 = 0;
		int   i;

		for (index1 = 0; index1 < 0x100; index1++)
		{
			for (index2 = index1, i = 0; i < 5; i++, index2 += 0x100)
			{
				unsigned long temp1, temp2;

				seed = (seed * 125 + 3) % 0x2AAAAB;
				temp1 = (seed & 0xFFFF) << 0x10;

				seed = (seed * 125 + 3) % 0x2AAAAB;
				temp2 = (seed & 0xFFFF);

				dwCryptTable[index2] = (temp1 | temp2);
			}
		}
	}

	unsigned int HashString(const char *lpszString, unsigned long dwHashType)
	{
		unsigned int seed1 = 0x7FED7FED;
		unsigned int seed2 = 0xEEEEEEEE;
		int    ch;

		while (*lpszString != 0)
		{
			ch = toupper(*lpszString++);

			seed1 = dwCryptTable[(dwHashType * 0xFF) + ch] ^ (seed1 + seed2);
			seed2 = ch + seed1 + seed2 + (seed2 << 5) + 3;
		}
		if (seed1 == 0){
			int a=1;
		}
		return seed1;
	}

	int GetHashTablePos(const char *lpszString, LPQHASHTABLE *lpHash, LPQBLOCKTABLE* lpBlock, int nTableSize)
	{
		auto nHash = HashString(lpszString, HASH_OFFSET);
		int hashPos = nHash%nTableSize;
		int blockPos = lpHash[hashPos].blockIndex;
		int endPos;
		if (hashPos < nTableSize - 1)
		{
			endPos = lpHash[hashPos + 1].blockIndex;
		}
		else
		{
			endPos = nTableSize;
		}
		if (endPos <= blockPos)
		{
			return -1;
		}
		auto nHashA = HashString(lpszString, HASH_A);
		auto nHashB = HashString(lpszString, HASH_B);
		for (int i = blockPos; i <endPos; i++)
		{
			if (lpBlock[i].nHashA == nHashA&&lpBlock[i].nHashB == nHashB)
			{
				return i;
			}
		}
		return -1;
	}


#ifdef __cplusplus
}
#endif

LPQFile::LPQFile() :
_file(nullptr)
{
	InitializeCryptTable();
}

LPQFile::~LPQFile()
{
	reset();
}

void LPQFile::reset()
{
	if (_file != nullptr)
	{
		fclose(_file);
		_file = nullptr;
	}
	LIR_SAFE_FREE(_hashTable);
	LIR_SAFE_DELETE(_blockTable);
}

FileStatus LPQFile::create(const std::string& packname, int version)
{
	FILE* file = nullptr;
	size_t size = 0;
	auto status = open(packname, "wb", file, size);
	if (status != FileStatus::Openend)
	{
		return status;
	}
	LPQHEADER header;
	header.head = LPQHEAD;
	header.version = version;
	header.fileCount = 0;
	header.contentSize = 0;
	header.hashOffset = LPQ_HEADER_SIZE;
	header.blockOffset = header.hashOffset+header.fileCount*sizeof(LPQHASHTABLE);

	fseek(file, 0, 0);
	int writeCount = fwrite(&_header, LPQ_HEADER_SIZE, 1, file);
	fclose(file);

	if (writeCount < 1)
	{	
		return FileStatus::WriteFailed;
	}
	return FileStatus::Success;
}

FileStatus LPQFile::openByPack(const std::string& packname, const char* mode)
{
	size_t size = 0;
	auto status = open(packname, mode, _file, size);
	if (status != FileStatus::Openend)
	{
		return status;
	}
	_header.fileCount = 0;
	_header.contentSize = 0;
	_header.hashOffset = 0;
	_header.blockOffset = 0;

	fseek(_file, 0, 0);
	if (size < LPQ_HEADER_SIZE)
	{
		//fclose(_file);
		//_file = nullptr;
		return FileStatus::NotInitialized;
	}
	if (fread(&_header, sizeof(LPQHEADER), 1, _file) < 1)
	{
		//fclose(_file);
		//_file = nullptr;
		return FileStatus::NotInitialized;
	}
	if (_header.fileCount>0)
	{
		fseek(_file,_header.hashOffset,0);
		size_t hashSize = sizeof(LPQHASHTABLE);
		size_t totoalHashSize = hashSize*_header.fileCount;
		_hashTable = (LPQHASHTABLE*)malloc(totoalHashSize);
		if (fread(_hashTable, hashSize, _header.fileCount, _file) < _header.fileCount)
		{
			return FileStatus::ReadFailed;
		}
		size_t blockSize = sizeof(LPQBLOCKTABLE);
		size_t totalBlockSize = blockSize*_header.fileCount;
		_blockTable = (LPQBLOCKTABLE*)malloc(totalBlockSize);
		if (fread(_blockTable, blockSize, _header.fileCount, _file) < _header.fileCount)
		{
			return FileStatus::ReadFailed;
		}
	}
	return FileStatus::Openend;
}

FileStatus LPQFile::read(const std::string& fileName, Buffer* buffer)
{
	if (_header.fileCount < 1)
	{
		return FileStatus::NotExists;
	}
	int index = GetHashTablePos(fileName.c_str(), _hashTable,_blockTable, _header.fileCount);
	if (index < 0)
	{
		return FileStatus::NotExists;
	}
	LPQBLOCKTABLE table = _blockTable[index];
	fseek(_file, table.fileOffset, 0);

	buffer->resize(table.fileSize);
	if (fread(buffer->buffer(), 1, table.fileSize, _file) < table.fileSize)
	{
		return FileStatus::ReadFailed;
	}
	return FileStatus::Success;
}

FileStatus LPQFile::resize(const int count)
{
	if (count>_header.fileCount)
	{
		auto totalHashSize = count*sizeof(LPQHASHTABLE);
		_hashTable = (LPQHASHTABLE*)realloc(_hashTable, totalHashSize);
		if (_hashTable == nullptr)
		{
			
			return FileStatus::WriteFailed;
		}
		memset(_hashTable + _header.fileCount, 0, sizeof(LPQHASHTABLE)*(count - _header.fileCount));

		auto totalBlockSize = count*sizeof(LPQBLOCKTABLE);
		_blockTable = (LPQBLOCKTABLE*)realloc(_blockTable, totalBlockSize);
		if (_blockTable == nullptr)
		{
			return FileStatus::WriteFailed;
		}
		memset(_blockTable + _header.fileCount, 0, sizeof(LPQBLOCKTABLE)*(count - _header.fileCount));
	}
	_header.fileCount = count;
	if (_header.hashOffset == 0 || _header.blockOffset == 0)
	{
		_header.blockOffset = _header.hashOffset = LPQ_HEADER_SIZE + _header.contentSize;
		fseek(_file, 0, 0);
		fwrite(&_header, LPQ_HEADER_SIZE, 1, _file);
	}
	return FileStatus::Success;
}

FileStatus LPQFile::append(const std::string& fullPath, const std::string& fileName, void* buff, size_t size,int index)
{
	const char* name = fileName.c_str();
	auto nHash = HashString(name, HASH_OFFSET);
	auto nHashA = HashString(name, HASH_A);
	auto nHashB = HashString(name, HASH_B);

	int blockIndex = index;
	if (blockIndex < 0)
	{
		for (int i = _header.fileCount-1; i >= 0; i--)
		{
			if (_blockTable[i].fileSize == 0 && (i == 0 || _blockTable[i].fileSize>0))
			{
				blockIndex = i;
				break;
			}
		}
	}
	if (blockIndex < 0 || blockIndex >= _header.fileCount)
	{
		return FileStatus::WriteFailed;
	}

	_blockTable[blockIndex].fileOffset = LPQ_HEADER_SIZE + _header.contentSize;
	_blockTable[blockIndex].fileSize = size;
	_blockTable[blockIndex].nHash = nHash;
	_blockTable[blockIndex].nHashA = nHashA;
	_blockTable[blockIndex].nHashB = nHashB;

	//fseek(_file, 0, 0);
	//fwrite(&_header, LPQ_HEADER_SIZE, 1, _file);

	if (fseek(_file, LPQ_HEADER_SIZE + _header.contentSize, 0) != 0)
	{
		return FileStatus::ReadFailed;
	}
	if (fwrite(buff, size, 1, _file) < 1)
	{
		return FileStatus::WriteFailed;
	}
	
	_header.contentSize += size;
	//_header.hashOffset = LPQ_HEADER_SIZE + _header.contentSize;
	//_header.blockOffset = _header.hashOffset + sizeof(LPQHASHTABLE)*_header.fileCount;
	return FileStatus::Success;
}

FileStatus LPQFile::flush()
{
	_header.hashOffset = LPQ_HEADER_SIZE + _header.contentSize;
	_header.blockOffset = _header.hashOffset + sizeof(LPQHASHTABLE)*_header.fileCount;

	auto totalBlockSize = _header.fileCount*sizeof(LPQBLOCKTABLE);
	LPQBLOCKTABLE* newBlockTable = (LPQBLOCKTABLE*)malloc(totalBlockSize);
	memset(newBlockTable, 0, totalBlockSize);

	memset(_hashTable, 0, sizeof(LPQHASHTABLE)*_header.fileCount);
	for (int i = 0; i < _header.fileCount; i++)
	{
		_hashTable[(_blockTable[i].nHash%_header.fileCount)].blockIndex++;
	}

	int totalCount = 0;
	int curCount = 0;
	for (int j = 0; j <_header.fileCount; j++)
	{
		curCount = _hashTable[j].blockIndex;
		totalCount += curCount;
		_hashTable[j].blockIndex = totalCount-curCount;
		lir::log("hash data %d %d %d \n", _blockTable[j].nHash, _blockTable[j].nHashA, _blockTable[j].nHashB);
		lir::log("_hashIndex %d  count=%d  startIndex= %d\n", j, curCount, _hashTable[j].blockIndex);
	}

	int startIndex = 0;
	int endIndex = 1;
	int index = 0;
	LPQBLOCKTABLE block;
	for (int l = 0; l < _header.fileCount; l++)
	{
		block = _blockTable[l];
		index = block.nHash%_header.fileCount;
		startIndex = _hashTable[index].blockIndex;
		if(startIndex<_header.fileCount)//index==filecoun NO FILE
		{
			if (index < _header.fileCount - 1)
			{
				endIndex = _hashTable[index + 1].blockIndex;
			}
			else
			{
				endIndex = _header.fileCount;
			}
			for (int k = startIndex; k < endIndex; k++)
			{
				if (newBlockTable[k].fileSize <= 0)
				{
					//memcpy(newBlockTable + k, &block, sizeof(LPQBLOCKTABLE));
					newBlockTable[k].fileOffset = block.fileOffset;
					newBlockTable[k].fileSize = block.fileSize;
					newBlockTable[k].nHashA = block.nHashA;
					newBlockTable[k].nHashB = block.nHashB;
					newBlockTable[k].nHash = block.nHash;
					break;
				}
			}
		}
	}
	free(_blockTable);
	_blockTable = newBlockTable;

	fseek(_file, 0, 0);
	if (fwrite(&_header, LPQ_HEADER_SIZE, 1, _file) < 1)
	{
		return FileStatus::WriteFailed;
	}
	if (fseek(_file, _header.hashOffset, 0) != 0)
	{
		return FileStatus::WriteFailed;
	}
	if (fwrite(_hashTable, sizeof(LPQHASHTABLE), _header.fileCount, _file) < _header.fileCount)
	{
		return FileStatus::WriteFailed;
	}
	if (fwrite(_blockTable, sizeof(LPQBLOCKTABLE), _header.fileCount, _file)<_header.fileCount)
	{
		return FileStatus::WriteFailed;
	}
	fclose(_file);
	_file = nullptr;
	return FileStatus::Success;
}

FileStatus LPQFile::write(const std::string& fullPath, const std::string& fileName, void* buff, size_t size)
{
	const char* name = fileName.c_str();
	int index = GetHashTablePos(name,_hashTable,_blockTable,_header.fileCount);
	if (index < 0)
	{	
		return append(fullPath, fileName, buff, size);
	}
	else{
		return FileStatus::WriteFailed;
	}

}




