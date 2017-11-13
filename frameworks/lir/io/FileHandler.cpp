#include "io/FileHandler.h"

//#include <stack>
#include <sys/stat.h>

USING_NS_LIR

FileStatus FileHandler::open(const std::string& fullPath, const char* mode, FILE* file, size_t& size)
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
	FILE* _file;
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
	
}


FileHandlerPack::FileHandlerPack() :
_file(nullptr)
{

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
	_header.fileCount = 0;
	LIR_SAFE_FREE(_hashTable);
}

FileStatus FileHandlerPack::openByPack(const std::string& packname, const char* mode)
{
	size_t size = 0;
	auto status = open(packname, mode, _file, size);
	if (status != FileStatus::Openend)
	{
		return status;
	}

	memset(&_header, 0, sizeof(MPQHEADER));

	fseek(_file, 0, 0);
	if (size < HEADER_SIZE)
	{
		return FileStatus::NotInitialized;
	}
	if (fread(&_header, 1, HEADER_SIZE, _file) < HEADER_SIZE)
	{
		return FileStatus::NotInitialized;
	}
	if (_header.fileCount>0)
	{
		size_t hashSize = sizeof(MPQHASHTABLE);
		size_t totoalHashSize = hashSize*_header.fileCount;
		_hashTable = (MPQHASHTABLE*)malloc(totoalHashSize);
		if (fread(_hashTable, hashSize, _header.fileCount, _file) < totoalHashSize)
		{
			return FileStatus::ReadFailed;
		}
		size_t blockSize = sizeof(MPQBLOCKTABLE);
		size_t totalBlockSize = blockSize*_header.fileCount;
		_blockTable = (MPQBLOCKTABLE*)malloc(totalBlockSize);
		if (fread(_blockTable, blockSize, _header.fileCount, _file) < totalBlockSize)
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


FileStatus FileHandlerPack::write(const std::string& fullPath, const std::string& fileName, void* buff, size_t size)
{
	const char* name = fileName.c_str();
	int index = GetHashTablePos(name,_hashTable,_blockTable,_header.fileCount);
	if (index < 0)
	{
	
		++_header.fileCount;

		fseek(_file, HEADER_SIZE + _header.contentSize, 0);
		fwrite(buff, size, 1, _file);
		_header.contentSize += size;

		//reinit hashTable
		auto totalHeaderSize = _header.fileCount*sizeof(MPQHASHTABLE);
		realloc(_hashTable, totalHeaderSize);

		size_t countSize = sizeof(unsigned int)*_header.fileCount;
		unsigned int* counts = (unsigned int*)malloc(countSize);
		memset(counts, 0, countSize);
		for (int i = 0; i < _header.fileCount;i++)
		{
			counts[i] += (_hashTable[i].hash%_header.fileCount);
		}
		
		int totalIndex = 0;
		for (int j = 0; j < _header.fileCount; j++)
		{
			_hashTable[j].blockIndex = totalIndex;
			totalIndex += counts[j];
		}


	

		return FileStatus::Success;
	}
}




#ifdef __cplusplus
extern "C" {
#endif


unsigned long dwCryptTable[0x500];

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

unsigned long HashString(const char *lpszString, unsigned long dwHashType)
{
	unsigned long  seed1 = 0x7FED7FED;
	unsigned long  seed2 = 0xEEEEEEEE;
	int    ch;

	while (*lpszString != 0)
	{
		ch = toupper(*lpszString++);

		seed1 = dwCryptTable[(dwHashType * 0xFF) + ch] ^ (seed1 + seed2);
		seed2 = ch + seed1 + seed2 + (seed2 << 5) + 3;
	}
	return seed1;
}

int GetHashTablePos(const char *lpszString, MPQHASHTABLE *lpHash,MPQBLOCKTABLE* lpBlock, int nTableSize)
{
	const int HASH_OFFSET = 0, HASH_A = 1, HASH_B = 2;
	int nHash = HashString(lpszString, HASH_OFFSET);
	int hashPos = nHash%nTableSize;
	int blockPos = lpHash[hashPos].blockIndex;
	int endPos;
	if (hashPos < nTableSize-1)
	{
		endPos = lpHash[hashPos + 1].blockIndex;
	}
	else
	{
		endPos = nTableSize;
	}
	int nHashA = HashString(lpszString, HASH_A);
	int nHashB = HashString(lpszString, HASH_B);
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