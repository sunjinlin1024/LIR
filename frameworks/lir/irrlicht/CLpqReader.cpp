// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
// Code contributed by skreamz

#include "CLpqReader.h"



#ifdef __IRR_COMPILE_WITH_PAK_ARCHIVE_LOADER_
//#include <sys/stat.h>
//#include "os.h"
//#include "coreutil.h"


namespace irr
{
namespace io
{

namespace
{

inline bool isHeaderValid(const LPQ_HEADER& header)
{
	return header.head == LPQHEAD;
}


#ifdef __cplusplus
extern "C" {
#endif

	bool lpqReaderInited = false;
	u32 lpqReaderCryptTable[0x500];

	void InitializeLpqReaderCryptTable()
	{
		u32 seed = 0x00100001;
		u32 index1 = 0;
		u32 index2 = 0;
		int   i;

		for (index1 = 0; index1 < 0x100; index1++)
		{
			for (index2 = index1, i = 0; i < 5; i++, index2 += 0x100)
			{
				u32 temp1, temp2;

				seed = (seed * 125 + 3) % 0x2AAAAB;
				temp1 = (seed & 0xFFFF) << 0x10;

				seed = (seed * 125 + 3) % 0x2AAAAB;
				temp2 = (seed & 0xFFFF);

				lpqReaderCryptTable[index2] = (temp1 | temp2);
			}
		}
	}
	
#ifdef __cplusplus
}
#endif



} // end namespace

//! Constructor
CArchiveLoaderLPQ::CArchiveLoaderLPQ( io::IFileSystem* fs)
: FileSystem(fs)
{
#ifdef _DEBUG
	setDebugName("CArchiveLoaderLPQ");
#endif
}


//! returns true if the file maybe is able to be loaded by this class
bool CArchiveLoaderLPQ::isALoadableFileFormat(const io::path& filename) const
{
	return core::hasFileExtension(filename, "lpq");
}

//! Check to see if the loader can create archives of this type.
bool CArchiveLoaderLPQ::isALoadableFileFormat(E_FILE_ARCHIVE_TYPE fileType) const
{
	return fileType == EFAT_LPQ;
}

//! Creates an archive from the filename
/** \param file File handle to check.
\return Pointer to newly created archive, or 0 upon error. */
IFileArchive* CArchiveLoaderLPQ::createArchive(const io::path& filename, bool ignoreCase, bool ignorePaths) const
{
	IFileArchive *archive = 0;
	io::IReadFile* file = FileSystem->createAndOpenFile(filename);

	if (file)
	{
		archive = createArchive(file, ignoreCase, ignorePaths);
		file->drop ();
	}

	return archive;
}

//! creates/loads an archive from the file.
//! \return Pointer to the created archive. Returns 0 if loading failed.
IFileArchive* CArchiveLoaderLPQ::createArchive(io::IReadFile* file, bool ignoreCase, bool ignorePaths) const
{
	IFileArchive *archive = 0;
	if ( file )
	{
		file->seek ( 0 );
		archive = new CLpqReader(file, ignoreCase, ignorePaths);

	}
	return archive;
}


//! Check if the file might be loaded by this class
/** Check might look into the file.
\param file File handle to check.
\return True if file seems to be loadable. */
bool CArchiveLoaderLPQ::isALoadableFileFormat(io::IReadFile* file) const
{
	LPQ_HEADER header;
	file->read(&header, sizeof(header));
	return isHeaderValid(header);
}


/*!
	LPQ Reader
*/
CLpqReader::CLpqReader(IReadFile* file, bool ignoreCase, bool ignorePaths)
//: IFileList( ignoreCase, ignorePaths), 
:File(file)
{
#ifdef _DEBUG
	setDebugName("CLpqReader");
#endif
	if (File)
	{
		File->grab();
		scanLocalHeader();
	}
}


CLpqReader::~CLpqReader()
{
	if (File)
		File->drop();
	if (_hashTable)
	{
		free(_hashTable);
		_hashTable = nullptr;
	}
		
	if (_blockTable) {
		free(_blockTable);
		_blockTable = nullptr;
	}
		
	if (_emptyTable) {
		free(_emptyTable);
		_emptyTable = nullptr;
	}
		
}


u32 CLpqReader::getFileCount() const
{
	return _header.fileCount;
}


const IFileList* CLpqReader::getFileList() const
{
	return this;
}

bool CLpqReader::scanLocalHeader()
{
	_header.fileCount = 0;
	_header.contentSize = 0;
	_header.hashOffset = 0;
	_header.blockOffset = 0;
	_header.emptyCount = 0;
	_header.emptyOffset = 0;

	auto size = File->getSize();
	if (size < LPQ_HEADER_SIZE)
		return false;

	// Read and validate the header
	File->seek(0);
	File->read(&_header, sizeof(_header));
	if (!isHeaderValid(_header))
		return false;
	if (_header.fileCount>0)
	{
		if (!File->seek(_header.hashOffset))
		{
			return false;
		}
		size_t hashSize = sizeof(LPQ_HASH_BLOCK);
		size_t totoalHashSize = hashSize*_header.fileCount;
		_hashTable = (LP_LPQ_HASH_TABLE)malloc(totoalHashSize);
		if (!File->read(_hashTable, totoalHashSize))
		{
			return false;
		}
		size_t blockSize = sizeof(LPQ_OFFSET_BLOCK);
		size_t totalBlockSize = blockSize*_header.fileCount;
		_blockTable = (LP_LPQ_OFFSET_TABLE)malloc(totalBlockSize);
		if (!File->read(_blockTable, totalBlockSize))
		{
			return false;
		}
		if (_header.emptyCount > 0)
		{
			size_t emptySize = sizeof(LPQ_OFFSET_BLOCK);
			size_t totalEmptySize = emptySize*_header.emptyCount;
			_emptyTable = (LP_LPQ_EMPTY_TABLE)malloc(totalEmptySize);
			if (!File->read(_emptyTable, totalEmptySize))
			{
				return false;
			}
		}
	}
	if (!lpqReaderInited)
	{
		lpqReaderInited = true;
		InitializeLpqReaderCryptTable();
	}
	return true;
}


//! opens a file by file name
IReadFile* CLpqReader::createAndOpenFile(const io::path& filename)
{
	s32 index = findFile(filename, false);

	if (index != -1)
		return createAndOpenFile(index);

	return 0;
}


//! opens a file by index
IReadFile* CLpqReader::createAndOpenFile(u32 index)
{
	if (index >= _header.fileCount)
		return 0;
	return createLimitReadFile( "", File, _blockTable[index].fileOffset, _blockTable[index].fileSize);

}

//! lpq now not support directory
bool CLpqReader::isDirectory(u32 index) const
{
	return false;
}


//! todo
const io::path& CLpqReader::getFileName(u32 index) const
{
	return io::path("");
}

//! todo
const io::path& CLpqReader::getFullFileName(u32 index) const
{
	return io::path("");
}


s32 CLpqReader::findFile(const io::path& filename, bool isFolder) const
{
	if (isFolder)
	{
		return _header.fileCount;
	}
	if (_header.fileCount < 1)
	{
		return -1;
	}
	auto nHash = hash(filename, 0);
	int hashPos = nHash%_header.fileCount;
	int blockPos = _hashTable[hashPos].blockIndex;
	int endPos;
	if (hashPos < _header.fileCount - 1)
	{
		endPos = _hashTable[hashPos + 1].blockIndex;
	}
	else
	{
		endPos = _header.fileCount;
	}
	if (endPos <= blockPos)
	{
		return -1;
	}
	auto nHashA = hash(filename, 1);
	auto nHashB = hash(filename, 2);
	for (int i = blockPos; i <endPos; i++)
	{
		if (_blockTable[i].nHashA == nHashA&&_blockTable[i].nHashB == nHashB)
		{
			return i;
		}
	}
	return -1;
}

u32 CLpqReader::hash(const io::path& lpszString, u32 dwHashType)const
{
	u32 seed1 = 0x7FED7FED;
	u32 seed2 = 0xEEEEEEEE;
	int    ch;

	auto size = lpszString.size();
	for(int i=0;i<size;i++){
		ch = irr::core::locale_upper(lpszString[i]);
		seed1 = lpqReaderCryptTable[(dwHashType * 0xFF) + ch] ^ (seed1 + seed2);
		seed2 = ch + seed1 + seed2 + (seed2 << 5) + 3;
	}
	return seed1;
}

u32 CLpqReader::getFileOffset(u32 index) const
{
	return _blockTable[index].fileOffset;
}

u32 CLpqReader::getFileSize(u32 index) const
{
	return _blockTable[index].fileSize;
}

//todo
u32 CLpqReader::getID(u32 index) const
{
	return 0;
}

} // end namespace io
} // end namespace irr

#endif // __IRR_COMPILE_WITH_PAK_ARCHIVE_LOADER_

