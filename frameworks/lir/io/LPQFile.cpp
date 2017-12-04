#include "io/LPQFile.h"

//#include <stack>
#include <sys/stat.h>

#include "debug/Debug.h"

USING_NS_LIR


#ifdef __cplusplus
extern "C" {
#endif

	bool inited = false;
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
			int a = 1;
		}
		return seed1;
	}

	int GetHashTablePos(const char *lpszString, LP_LPQ_HASH_TABLE lpHash, LP_LPQ_OFFSET_TABLE lpBlock, int nTableSize)
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



LPQFile::LPQFile(BaseFile* file){
	if (!inited)
	{
		InitializeCryptTable();
		inited = true;
	}
	_file = file;
}


LPQFile::~LPQFile(){
	//reset();
	if (_file)
	{
		_file->close();
		LIR_SAFE_DELETE(_file);
	}
}


void LPQFile::reset()
{
	_file->close();
	LIR_SAFE_FREE(_hashTable);
	LIR_SAFE_FREE(_blockTable);
	LIR_SAFE_FREE(_emptyTable);
}

FileStatus LPQFile::create(const std::string& packname, int version)
{
	FILE* file = nullptr;
	
	FileStatus status = _file->open(packname, "wb");
	if (status != FileStatus::Success)
	{
		return status;
	}
	size_t size = 0;
	_file->getSize(size);

	LPQ_HEADER header;
	header.head = LPQHEAD;
	header.version = version;
	header.fileCount = 0;
	header.contentSize = 0;
	header.hashOffset = LPQ_HEADER_SIZE;
	header.blockOffset = header.hashOffset+header.fileCount*sizeof(LPQ_HASH_BLOCK);
	header.emptyCount = 0;
	header.emptyOffset = header.blockOffset + header.fileCount*sizeof(LPQ_OFFSET_BLOCK);

	fseek(file, 0, 0);
	_file->write(&_header, LPQ_HEADER_SIZE, 1);
	//fclose(file);
	_file->close();
	return FileStatus::Success;
}

FileStatus LPQFile::openLPQ(const std::string& packname, const char* mode)
{
	
	FileStatus status = _file->open(packname, mode);
	if (status != FileStatus::Success)
	{
		return status;
	}
	size_t size = 0;
	_file->getSize(size);

	_header.fileCount = 0;
	_header.contentSize = 0;
	_header.hashOffset = 0;
	_header.blockOffset = 0;
	_header.emptyCount = 0;
	_header.emptyOffset = 0;

	_file->seek( 0, 0);
	if (size < LPQ_HEADER_SIZE)
	{
		//fclose(_file);
		//_file = nullptr;
		return FileStatus::NotInitialized;
	}
	if (_file->read(&_header, sizeof(LPQ_HEADER), 1))
	{
		//fclose(_file);
		//_file = nullptr;
		return FileStatus::NotInitialized;
	}
	if (_header.fileCount>0)
	{
		if (_file->seek(_header.hashOffset, 0))
		{
			return FileStatus::SeekFailed;
		}
		size_t hashSize = sizeof(LPQ_HASH_BLOCK);
		size_t totoalHashSize = hashSize*_header.fileCount;
		_hashTable = (LP_LPQ_HASH_TABLE)malloc(totoalHashSize);
		if (_file->read(_hashTable, hashSize, _header.fileCount))
		{
			return FileStatus::ReadFailed;
		}
		size_t blockSize = sizeof(LPQ_OFFSET_BLOCK);
		size_t totalBlockSize = blockSize*_header.fileCount;
		_blockTable = (LP_LPQ_OFFSET_TABLE)malloc(totalBlockSize);
		if (_file->read(_blockTable, blockSize, _header.fileCount))
		{
			return FileStatus::ReadFailed;
		}
		if (_header.emptyCount > 0)
		{
			size_t emptySize = sizeof(LPQ_OFFSET_BLOCK);
			size_t totalEmptySize = emptySize*_header.emptyCount;
			_emptyTable = (LP_LPQ_EMPTY_TABLE)malloc(totalEmptySize);
			if (_file->read(_emptyTable, emptySize, _header.emptyCount))
			{
				return FileStatus::ReadFailed;
			}
		}
	}
	return FileStatus::Success;
}
FileStatus LPQFile::exists(const std::string& fileName)
{
	if (_header.fileCount < 1)
	{
		return FileStatus::NotExists;
	}
	int index = GetHashTablePos(fileName.c_str(), _hashTable, _blockTable, _header.fileCount);
	if (index < 0)
	{
		return FileStatus::NotExists;
	}
	return FileStatus::Success;
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
	LPQ_OFFSET_BLOCK table = _blockTable[index];
	_file->seek(table.fileOffset, 0);

	buffer->resize(table.fileSize);
	if (_file->read(buffer->buffer(), 1, table.fileSize))
	{
		return FileStatus::ReadFailed;
	}
	return FileStatus::Success;
}

FileStatus LPQFile::resize(const int count)
{
	if (count>_header.fileCount)
	{
		auto totalHashSize = count*sizeof(LPQ_HASH_BLOCK);
		_hashTable = (LP_LPQ_HASH_TABLE)realloc(_hashTable, totalHashSize);
		if (_hashTable == nullptr)
		{
			
			return FileStatus::WriteFailed;
		}
		memset(_hashTable + _header.fileCount, 0, sizeof(LPQ_HASH_BLOCK)*(count - _header.fileCount));

		auto totalBlockSize = count*sizeof(LPQ_OFFSET_BLOCK);
		_blockTable = (LP_LPQ_OFFSET_TABLE)realloc(_blockTable, totalBlockSize);
		if (_blockTable == nullptr)
		{
			return FileStatus::WriteFailed;
		}
		memset(_blockTable + _header.fileCount, 0, sizeof(LPQ_OFFSET_BLOCK)*(count - _header.fileCount));
	}
	_header.fileCount = count;
	if (_header.hashOffset == 0 || _header.blockOffset == 0||_header.emptyOffset==0)
	{
		_header.blockOffset = _header.hashOffset = LPQ_HEADER_SIZE + _header.contentSize;
		_file->seek(0, 0);
		_file->write(&_header, LPQ_HEADER_SIZE, 1);
	}
	return FileStatus::Success;
}

FileStatus LPQFile::append(const std::string& fileName, void* buff, size_t size,int index)
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

	if (_file->seek(LPQ_HEADER_SIZE + _header.contentSize, 0))
	{
		return FileStatus::ReadFailed;
	}
	if (_file->write(buff, size, 1))
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
	_header.blockOffset = _header.hashOffset + sizeof(LPQ_HASH_BLOCK)*_header.fileCount;

	auto totalBlockSize = _header.fileCount*sizeof(LPQ_OFFSET_BLOCK);
	LP_LPQ_OFFSET_TABLE newBlockTable = (LP_LPQ_OFFSET_TABLE)malloc(totalBlockSize);
	memset(newBlockTable, 0, totalBlockSize);

	memset(_hashTable, 0, sizeof(LPQ_HASH_BLOCK)*_header.fileCount);
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
		//lir::log("hash data %d %d %d \n", _blockTable[j].nHash, _blockTable[j].nHashA, _blockTable[j].nHashB);
		//lir::log("_hashIndex %d  count=%d  startIndex= %d\n", j, curCount, _hashTable[j].blockIndex);
	}

	int startIndex = 0;
	int endIndex = 1;
	int index = 0;
	LPQ_OFFSET_BLOCK block;
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


	_file->seek(0, 0);
	if (_file->write(&_header, LPQ_HEADER_SIZE, 1))
	{
		return FileStatus::WriteFailed;
	}
	if (_header.fileCount>0)
	{
		if (_file->seek(_header.hashOffset, 0))
		{
			return FileStatus::WriteFailed;
		}
		if (_file->write(_hashTable, sizeof(LPQ_HASH_BLOCK), _header.fileCount))
		{
			return FileStatus::WriteFailed;
		}
		if (_file->write(_blockTable, sizeof(LPQ_OFFSET_BLOCK), _header.fileCount))
		{
			return FileStatus::WriteFailed;
		}
		if (_header.emptyCount>0)
		{
			if (_file->write(_emptyTable, sizeof(LPQ_EMPTY_BLOCK), _header.emptyCount))
			{
				return FileStatus::WriteFailed;
			}
		}
	}
	//_file->close();
	//LIR_SAFE_DELETE(_file);
	//_file = nullptr;
	return FileStatus::Success;
}

FileStatus LPQFile::write(const std::string& fileName, void* buff, size_t size)
{
	const char* name = fileName.c_str();
	int index = -1;
	if (_header.fileCount > 0)
	{
		index=GetHashTablePos(name, _hashTable, _blockTable, _header.fileCount);
	}
	if (index < 0)
	{	
		auto result = this->resize(_header.fileCount + 1);
		if (result != FileStatus::Success)
		{
			return result;
		}
		result=append( fileName, buff, size);
		if (result != FileStatus::Success)
		{
			return result;
		}
		return flush();
	}
	else{
		LPQ_OFFSET_BLOCK block = _blockTable[index];
		UINT oldSize = block.fileSize;
		UINT oldFileOffset = block.fileOffset;

		UINT offset = 0;
		if (oldSize == size)//写在原来位置
		{
			offset = oldFileOffset;
		}
		else//写在空白处
		{
			block.fileSize = size;

			int emptyIndex = -1;
			UINT curDiff = 999999999;
			for (UINT i = 0; i < _header.emptyCount; i++)
			{
				if (_emptyTable[i].size >= size)
				{
					if (_emptyTable[i].size - size < curDiff)
					{
						emptyIndex = i;
						curDiff = _emptyTable[i].size - size;
						if (curDiff == 0)
						{
							break;
						}
					}
				}
			}
			if (curDiff == 0)
			{
				offset = block.fileOffset = _emptyTable[emptyIndex].offset;

				_emptyTable[emptyIndex].offset = oldFileOffset;
				_emptyTable[emptyIndex].size = oldSize;
			}
			else{
				_header.emptyCount++;
				_emptyTable = (LP_LPQ_EMPTY_TABLE)realloc(_emptyTable, _header.emptyCount*sizeof(LPQ_EMPTY_BLOCK));
				_emptyTable[_header.emptyCount - 1].offset = oldFileOffset;
				_emptyTable[_header.emptyCount - 1].size = oldSize;

				if (emptyIndex >= 0)
				{
					offset = block.fileOffset = _emptyTable[emptyIndex].offset;

					_emptyTable[emptyIndex].offset += size;
					_emptyTable[emptyIndex].size -= size;
				}
				else//写在尾巴上
				{
					offset = LPQ_HEADER_SIZE + _header.contentSize;
					_header.contentSize += size;
					_header.hashOffset = LPQ_HEADER_SIZE + _header.contentSize;
					_header.blockOffset = _header.hashOffset + _header.fileCount*sizeof(LPQ_HASH_BLOCK);
					_header.emptyOffset = _header.blockOffset + _header.fileCount*sizeof(LPQ_OFFSET_BLOCK);
				}
			}
		}

		if (_file->seek(offset, 0)!=FileStatus::Success)
		{
			return FileStatus::ReadFailed;
		}
		if (_file->write(buff, size, 1)!=FileStatus::Success)
		{
			return FileStatus::WriteFailed;
		}
		return flush();
	}

}




