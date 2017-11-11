
#include "io/LFileUtils.h"

#include <stack>

//#include "base/CCData.h"
#include "core/LMacros.h"
//#include "platform/CCSAXParser.h"
//#include "base/ccUtils.h"

//#include "tinyxml2/tinyxml2.h"
//#include "tinydir/tinydir.h"
//#ifdef MINIZIP_FROM_SYSTEM
//#include <minizip/unzip.h>
//#else // from our embedded sources
//#include "unzip.h"
//#endif
#include <sys/stat.h>
#include <sys/types.h>

USING_NS_LIR

// Implement DictMaker

#if (LIR_TARGET_PLATFORM != LIR_PLATFORM_IOS) && (LIR_TARGET_PLATFORM != LIR_PLATFORM_MAC)

std::string FileUtils::getSuitableFOpen(const std::string& filenameUtf8) const
{
	LASSERT(false, "getSuitableFOpen should be override by platform FileUtils");
	return filenameUtf8;
}

#else

#endif /* (LIR_TARGET_PLATFORM != LIR_PLATFORM_IOS) && (LIR_TARGET_PLATFORM != LIR_PLATFORM_MAC) */


static bool s_popupNotify = true;

void FileUtils::setPopupNotify(bool notify)
{
	s_popupNotify = notify;
}

bool FileUtils::isPopupNotify() const
{
	return s_popupNotify;
}

// Implement FileUtils
FileUtils* FileUtils::s_sharedFileUtils = nullptr;


void FileUtils::destroyInstance()
{
    LIR_SAFE_DELETE(s_sharedFileUtils);
}

void FileUtils::setDelegate(FileUtils *delegate)
{
    if (s_sharedFileUtils)
        delete s_sharedFileUtils;
    s_sharedFileUtils = delegate;
}

FileUtils::FileUtils()
    : _writablePath("")
{
}

FileUtils::~FileUtils()
{
}

bool FileUtils::init()
{
	_searchPathArray.push_back(_defaultResRootPath);
	_searchResolutionsOrderArray.push_back("");
	return true;
}

void FileUtils::purgeCachedEntries()
{
	_fullPathCache.clear();
}

FileUtils::Status FileUtils::getContents(const std::string& filename, Buffer* buffer)
{
	if (filename.empty())
		return Status::NotExists;

	auto fs = FileUtils::getInstance();

	std::string fullPath = fs->fullPathForFilename(filename);
	if (fullPath.empty())
		return Status::NotExists;

	FILE *fp = fopen(fs->getSuitableFOpen(fullPath).c_str(), "rb");
	if (!fp)
		return Status::OpenFailed;

#if defined(_MSC_VER)
	auto descriptor = _fileno(fp);
#else
	auto descriptor = fileno(fp);
#endif
	struct stat statBuf;
	if (fstat(descriptor, &statBuf) == -1) {
		fclose(fp);
		return Status::ReadFailed;
	}
	size_t size = statBuf.st_size;

	buffer->resize(size);
	size_t readsize = fread(buffer->buffer(), 1, size, fp);
	fclose(fp);

	if (readsize < size) {
		buffer->resize(readsize);
		return Status::ReadFailed;
	}

	return Status::OK;
}

std::string FileUtils::getPathForFilename(const std::string& filename, const std::string& resolutionDirectory, const std::string& searchPath) const
{
	std::string file = filename;
	std::string file_path = "";
	size_t pos = filename.find_last_of("/");
	if (pos != std::string::npos)
	{
		file_path = filename.substr(0, pos + 1);
		file = filename.substr(pos + 1);
	}

	// searchPath + file_path + resourceDirectory
	std::string path = searchPath;
	path += file_path;
	path += resolutionDirectory;

	path = getFullPathForDirectoryAndFilename(path, file);

	return path;
}

std::string FileUtils::getNewFilename(const std::string &filename) const
{
	//std::string newFileName;

	//// in Lookup Filename dictionary ?
	//auto iter = _filenameLookupDict.find(filename);

	//if (iter == _filenameLookupDict.end())
	//{
	//	newFileName = filename;
	//}
	//else
	//{
	//	newFileName = iter->second.asString();
	//}
	//return newFileName;
	return filename;
}

std::string FileUtils::fullPathForFilename(const std::string &filename) const
{
	if (filename.empty())
	{
		return "";
	}

	if (isAbsolutePath(filename))
	{
		return filename;
	}

	// Already Cached ?
	auto cacheIter = _fullPathCache.find(filename);
	if (cacheIter != _fullPathCache.end())
	{
		return cacheIter->second;
	}

	// Get the new file name.
	const std::string newFilename(getNewFilename(filename));

	std::string fullpath;

	for (const auto& searchIt : _searchPathArray)
	{
		for (const auto& resolutionIt : _searchResolutionsOrderArray)
		{
			fullpath = this->getPathForFilename(newFilename, resolutionIt, searchIt);

			if (!fullpath.empty())
			{
				// Using the filename passed in as key.
				_fullPathCache.emplace(filename, fullpath);
				return fullpath;
			}

		}
	}

	if (isPopupNotify()){
		CCLOG("cocos2d: fullPathForFilename: No file found at %s. Possible missing file.", filename.c_str());
	}

	// The file wasn't found, return empty string.
	return "";
}



