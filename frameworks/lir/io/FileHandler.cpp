#include "io/FileHandler.h"

//#include <stack>
#include <sys/stat.h>

#include "debug/Debug.h"

USING_NS_LIR

FileStatus FileHandler::open(const std::string& fullPath, const char* mode, FILE* &file, size_t& size)
{
	file = fopen(fullPath.c_str(), mode);
	if (!file)
		return FileStatus::OpenFailed;
#if defined(_MSC_VER)
	auto descriptor = _fileno(file);
#else
	auto descriptor = fileno(fp);
#endif
	struct stat statBuf;
	if (fstat(descriptor, &statBuf) == -1) {
		fclose(file);
		file = nullptr;
		return FileStatus::ReadFailed;
	}
	size = statBuf.st_size;
	return FileStatus::Openend;
}



FileHandlerSingle::FileHandlerSingle()
{
	//_file = nullptr;
}

FileHandlerSingle::~FileHandlerSingle()
{
	//if (_file!=nullptr)
	//{
	//	fclose(_file);
	//	_file = nullptr;
	//}
}

FileStatus FileHandlerSingle::read(const std::string& filename, Buffer* buffer)
{
	size_t size = 0;
	FILE* _file=nullptr;
	auto status=open(filename,"rb", _file, size);
	if (status != FileStatus::Openend)
	{
		return status;
	}
	buffer->resize(size);
	size_t readsize = fread(buffer->buffer(), 1, size, _file);
	fclose(_file);
	_file = nullptr;

	if (readsize < size) {
		buffer->resize(readsize);
		return FileStatus::ReadFailed;
	}
	return FileStatus::Success;
}


FileStatus FileHandlerSingle::write(const std::string& fullPath, void* buff, size_t size)
{
	//todo
	return FileStatus::Success;
}


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
		return seed1;
	}

	int GetHashTablePos(const char *lpszString, MPQHASHTABLE *lpHash, MPQBLOCKTABLE* lpBlock, int nTableSize)
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

FileHandlerPack::FileHandlerPack() :
_file(nullptr)
{
	InitializeCryptTable();
}

FileHandlerPack::~FileHandlerPack()
{
	reset();
}

void FileHandlerPack::reset()
{
	if (_file != nullptr)
	{
		fclose(_file);
		_file = nullptr;
	}
	LIR_SAFE_FREE(_hashTable);
	LIR_SAFE_DELETE(_blockTable);
}

FileStatus FileHandlerPack::create(const std::string& packname, int version)
{
	FILE* file = nullptr;
	size_t size = 0;
	auto status = open(packname, "wb", file, size);
	if (status != FileStatus::Openend)
	{
		return status;
	}
	MPQHEADER header;
	header.head = MPQ_HEADER;
	header.version = version;
	header.fileCount = 0;
	header.contentSize = 0;
	header.hashOffset = MPQ_HEADER_SIZE;
	header.blockOffset = header.hashOffset+header.fileCount*sizeof(MPQHASHTABLE);

	fseek(file, 0, 0);
	int writeCount = fwrite(&_header, MPQ_HEADER_SIZE, 1, file);
	fclose(file);

	if (writeCount < 1)
	{	
		return FileStatus::WriteFailed;
	}
	return FileStatus::Success;
}

FileStatus FileHandlerPack::openByPack(const std::string& packname, const char* mode)
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
	if (size < MPQ_HEADER_SIZE)
	{
		//fclose(_file);
		//_file = nullptr;
		return FileStatus::NotInitialized;
	}
	if (fread(&_header, sizeof(MPQHEADER), 1, _file) < 1)
	{
		//fclose(_file);
		//_file = nullptr;
		return FileStatus::NotInitialized;
	}
	if (_header.fileCount>0)
	{
		fseek(_file,_header.hashOffset,0);
		size_t hashSize = sizeof(MPQHASHTABLE);
		size_t totoalHashSize = hashSize*_header.fileCount;
		_hashTable = (MPQHASHTABLE*)malloc(totoalHashSize);
		if (fread(_hashTable, hashSize, _header.fileCount, _file) < _header.fileCount)
		{
			return FileStatus::ReadFailed;
		}
		size_t blockSize = sizeof(MPQBLOCKTABLE);
		size_t totalBlockSize = blockSize*_header.fileCount;
		_blockTable = (MPQBLOCKTABLE*)malloc(totalBlockSize);
		if (fread(_blockTable, blockSize, _header.fileCount, _file) < _header.fileCount)
		{
			return FileStatus::ReadFailed;
		}
	}
	return FileStatus::Openend;
}

FileStatus FileHandlerPack::read(const std::string& fileName, Buffer* buffer)
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
	MPQBLOCKTABLE table = _blockTable[index];
	fseek(_file, table.fileOffset, 0);

	buffer->resize(table.fileSize);
	if (fread(buffer->buffer(), 1, table.fileSize, _file) < table.fileSize)
	{
		return FileStatus::ReadFailed;
	}
	return FileStatus::Success;
}

int FileHandlerPack::getCount()
{
	return _header.fileCount;
}

FileStatus FileHandlerPack::resize(const int count)
{
	if (count>_header.fileCount)
	{
		auto totalHashSize = count*sizeof(MPQHASHTABLE);
		_hashTable = (MPQHASHTABLE*)realloc(_hashTable, totalHashSize);
		if (_hashTable == nullptr)
		{
			
			return FileStatus::WriteFailed;
		}
		memset(_hashTable + _header.fileCount, 0, sizeof(MPQHASHTABLE)*(count - _header.fileCount));

		auto totalBlockSize = count*sizeof(MPQBLOCKTABLE);
		_blockTable = (MPQBLOCKTABLE*)realloc(_blockTable, totalBlockSize);
		if (_blockTable == nullptr)
		{
			return FileStatus::WriteFailed;
		}
		memset(_blockTable + _header.fileCount, 0, sizeof(MPQBLOCKTABLE)*(count - _header.fileCount));
	}
	_header.fileCount = count;
	if (_header.hashOffset == 0 || _header.blockOffset == 0)
	{
		_header.blockOffset = _header.hashOffset = MPQ_HEADER_SIZE + _header.contentSize;
		fseek(_file, 0, 0);
		fwrite(&_header, MPQ_HEADER_SIZE, 1, _file);
	}
	return FileStatus::Success;
}

FileStatus FileHandlerPack::append(const std::string& fullPath, const std::string& fileName, void* buff, size_t size,int index)
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

	_blockTable[blockIndex].fileOffset = MPQ_HEADER_SIZE + _header.contentSize;
	_blockTable[blockIndex].fileSize = size;
	_blockTable[blockIndex].nHash = nHash;
	_blockTable[blockIndex].nHashA = nHashA;
	_blockTable[blockIndex].nHashB = nHashB;

	//fseek(_file, 0, 0);
	//fwrite(&_header, MPQ_HEADER_SIZE, 1, _file);

	if (fseek(_file, MPQ_HEADER_SIZE + _header.contentSize, 0) != 0)
	{
		return FileStatus::ReadFailed;
	}
	if (fwrite(buff, size, 1, _file) < 1)
	{
		return FileStatus::WriteFailed;
	}
	
	_header.contentSize += size;
	//_header.hashOffset = MPQ_HEADER_SIZE + _header.contentSize;
	//_header.blockOffset = _header.hashOffset + sizeof(MPQHASHTABLE)*_header.fileCount;
	return FileStatus::Success;
}

FileStatus FileHandlerPack::flush()
{
	_header.hashOffset = MPQ_HEADER_SIZE + _header.contentSize;
	_header.blockOffset = _header.hashOffset + sizeof(MPQHASHTABLE)*_header.fileCount;

	auto totalBlockSize = _header.fileCount*sizeof(MPQBLOCKTABLE);
	MPQBLOCKTABLE* newBlockTable = (MPQBLOCKTABLE*)malloc(totalBlockSize);
	memset(newBlockTable, 0, totalBlockSize);

	memset(_hashTable, 0, sizeof(MPQHASHTABLE)*_header.fileCount);
	for (int i = 0; i < _header.fileCount; i++)
	{
		_hashTable[(_blockTable[i].nHash%_header.fileCount)].blockIndex++;
	}

	int totalIndex = 0;
	for (int j = 0; j <_header.fileCount; j++)
	{
		if (_hashTable[j].blockIndex>0)
		{
			_hashTable[j].blockIndex = totalIndex;
			totalIndex += _hashTable[j].blockIndex;
		}
		else
		{
			_hashTable[j].blockIndex = _header.fileCount;
		}
		lir::log("hash data %u %u %u \n", _blockTable[j].nHash, _blockTable[j].nHashA, _blockTable[j].nHashB);
		lir::log("_hashIndex %d = %d \n", j, _hashTable[j].blockIndex);
	}

	int startIndex = 0;
	int endIndex = 1;
	int index = 0;
	MPQBLOCKTABLE block;
	for (int l = 0; l < _header.fileCount; l++)
	{
		block = _blockTable[l];
		index = block.nHash%_header.fileCount;
		startIndex = _hashTable[index].blockIndex;
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
				//memcpy(newBlockTable + k, &block, sizeof(MPQBLOCKTABLE));
				newBlockTable[k].fileOffset = block.fileOffset;
				newBlockTable[k].fileSize = block.fileSize;
				newBlockTable[k].nHashA = block.nHashA;
				newBlockTable[k].nHashB = block.nHashB;
				newBlockTable[k].nHash = block.nHash;
				break;
			}
		}
	}
	free(_blockTable);
	_blockTable = newBlockTable;

	fseek(_file, 0, 0);
	if (fwrite(&_header, MPQ_HEADER_SIZE, 1, _file) < 1)
	{
		return FileStatus::WriteFailed;
	}
	if (fseek(_file, _header.hashOffset, 0) != 0)
	{
		return FileStatus::WriteFailed;
	}
	if (fwrite(_hashTable, sizeof(MPQHASHTABLE), _header.fileCount, _file) < _header.fileCount)
	{
		return FileStatus::WriteFailed;
	}
	if (fwrite(_blockTable, sizeof(MPQBLOCKTABLE), _header.fileCount, _file)<_header.fileCount)
	{
		return FileStatus::WriteFailed;
	}
	fclose(_file);
	_file = nullptr;
	return FileStatus::Success;
}

FileStatus FileHandlerPack::write(const std::string& fullPath, const std::string& fileName, void* buff, size_t size)
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




