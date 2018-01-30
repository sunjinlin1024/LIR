// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __C_LPQ_READER_H_INCLUDED__
#define __C_LPQ_READER_H_INCLUDED__

#include "IrrCompileConfig.h"

#ifdef __IRR_COMPILE_WITH_LPQ_ARCHIVE_LOADER_

#include "IReferenceCounted.h"
#include "IReadFile.h"
#include "irrArray.h"
#include "irrString.h"
#include "IFileSystem.h"
#include "IFileList.h"

namespace irr
{
namespace io
{
	#define LPQHEAD 0xABCC
	#define LPQCURRENT_VERSION 1

	const int LPQ_HEADER_SIZE = 208;

	struct IRRLICHT_API LPQ_HEADER
	{
		u32 head = LPQHEAD;
		u32 version = LPQCURRENT_VERSION;
		u32 fileCount;
		u32 contentSize;
		u32 hashOffset;
		u32 blockOffset;
		u32 emptyCount;
		u32 emptyOffset;
		u32 extra[LPQ_HEADER_SIZE / sizeof(unsigned int) - 8];
	};

	typedef struct IRRLICHT_API LPQ_HASH_BLOCK
	{
		u32 blockIndex;
	}*LP_LPQ_HASH_TABLE;

	typedef struct IRRLICHT_API LPQ_OFFSET_BLOCK
	{
		u32 nHash;
		u32 nHashA;
		u32 nHashB;
		u32 fileOffset;
		u32 fileSize;
	}*LP_LPQ_OFFSET_TABLE;

	typedef struct IRRLICHT_API LPQ_EMPTY_BLOCK
	{
		u32 offset;
		u32 size;
	}*LP_LPQ_EMPTY_TABLE;

	//! Archiveloader capable of loading PAK Archives
	class CArchiveLoaderLPQ : public IArchiveLoader
	{
	public:

		//! Constructor
		CArchiveLoaderLPQ(io::IFileSystem* fs);

		//! returns true if the file maybe is able to be loaded by this class
		//! based on the file extension (e.g. ".lpq")
		virtual bool isALoadableFileFormat(const io::path& filename) const;

		//! Check if the file might be loaded by this class
		/** Check might look into the file.
		\param file File handle to check.
		\return True if file seems to be loadable. */
		virtual bool isALoadableFileFormat(io::IReadFile* file) const;

		//! Check to see if the loader can create archives of this type.
		/** Check based on the archive type.
		\param fileType The archive type to check.
		\return True if the archile loader supports this type, false if not */
		virtual bool isALoadableFileFormat(E_FILE_ARCHIVE_TYPE fileType) const;

		//! Creates an archive from the filename
		/** \param file File handle to check.
		\return Pointer to newly created archive, or 0 upon error. */
		virtual IFileArchive* createArchive(const io::path& filename, bool ignoreCase, bool ignorePaths) const;

		//! creates/loads an archive from the file.
		//! \return Pointer to the created archive. Returns 0 if loading failed.
		virtual io::IFileArchive* createArchive(io::IReadFile* file, bool ignoreCase, bool ignorePaths) const;

		//! Returns the type of archive created by this loader
		virtual E_FILE_ARCHIVE_TYPE getType() const { return EFAT_LPQ; }

	private:
		io::IFileSystem* FileSystem;
	};


	//! reads from lpq
	class CLpqReader : public virtual IFileArchive, virtual IFileList
	{
	public:

		CLpqReader(IReadFile* file, bool ignoreCase, bool ignorePaths);
		virtual ~CLpqReader();

		// file archive methods

		//! return the id of the file Archive
		virtual const io::path& getArchiveName() const
		{
			return File->getFileName();
		}

		//! opens a file by file name
		virtual IReadFile* createAndOpenFile(const io::path& filename);

		//! opens a file by index
		virtual IReadFile* createAndOpenFile(u32 index);

		//! returns the list of files
		virtual const IFileList* getFileList() const;

		//! get the class Type
		virtual E_FILE_ARCHIVE_TYPE getType() const { return EFAT_LPQ; }

		//! Get the number of files in the filelist.
		/** \return Amount of files and directories in the file list. */
		virtual u32 getFileCount() const;

		//! Gets the name of a file in the list, based on an index.
		/** The path is not included in this name. Use getFullFileName for this.
		\param index is the zero based index of the file which name should
		be returned. The index must be less than the amount getFileCount() returns.
		\return File name of the file. Returns 0, if an error occured. */
		virtual const io::path& getFileName(u32 index) const;

		//! Gets the full name of a file in the list including the path, based on an index.
		/** \param index is the zero based index of the file which name should
		be returned. The index must be less than the amount getFileCount() returns.
		\return File name of the file. Returns 0 if an error occured. */
		virtual const io::path& getFullFileName(u32 index) const;

		//! Returns the size of a file in the file list, based on an index.
		/** \param index is the zero based index of the file which should be returned.
		The index must be less than the amount getFileCount() returns.
		\return The size of the file in bytes. */
		virtual u32 getFileSize(u32 index) const;

		//! Returns the file offset of a file in the file list, based on an index.
		/** \param index is the zero based index of the file which should be returned.
		The index must be less than the amount getFileCount() returns.
		\return The offset of the file in bytes. */
		virtual u32 getFileOffset(u32 index) const;

		//! Returns the ID of a file in the file list, based on an index.
		/** This optional ID can be used to link the file list entry to information held
		elsewhere. For example this could be an index in an IFileArchive, linking the entry
		to its data offset, uncompressed size and CRC.
		\param index is the zero based index of the file which should be returned.
		The index must be less than the amount getFileCount() returns.
		\return The ID of the file. */
		virtual u32 getID(u32 index) const;

		//! Check if the file is a directory
		/** \param index The zero based index which will be checked. The index
		must be less than the amount getFileCount() returns.
		\return True if the file is a directory, else false. */
		virtual bool isDirectory(u32 index) const;

		//! Searches for a file or folder in the list
		/** Searches for a file by name
		\param filename The name of the file to search for.
		\param isFolder True if you are searching for a directory path, false if you are searching for a file
		\return Returns the index of the file in the file list, or -1 if
		no matching name name was found. */
		virtual s32 findFile(const io::path& filename, bool isFolder = false) const;

		//! Returns the base path of the file list
		virtual const io::path& getPath() const { return File->getFileName();};

		//! Add as a file or folder to the list
		virtual u32 addItem(const io::path& fullPath, u32 offset, u32 size, bool isDirectory, u32 id = 0) { return 0; };

		//! Sorts the file list. You should call this after adding any items to the file list
		virtual void sort() {};

	private:

		//! scans for a local header, returns false if the header is invalid
		bool scanLocalHeader();
		u32 hash(const io::path& lpszString, u32 drawType=0)const;

		IReadFile* File;
		io::path Path;

		LPQ_HEADER _header;
		LP_LPQ_HASH_TABLE _hashTable;
		LP_LPQ_OFFSET_TABLE _blockTable;
		LP_LPQ_EMPTY_TABLE _emptyTable;
	};

} // end namespace io
} // end namespace irr

#endif // __IRR_COMPILE_WITH_PAK_ARCHIVE_LOADER_

#endif // __C_PAK_READER_H_INCLUDED__

