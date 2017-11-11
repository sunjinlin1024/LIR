#include "io/FileReader.h"

#include <stack>


#include <sys/stat.h>

USING_NS_LIR


FileReader::FileReader()
{

}

FileReader::~FileReader()
{

}


FileStatus FileReader::openFile(const std::string& filename, FILE* file,size_t& size )
{
	file = fopen(filename.c_str(), "rb");
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



FileReaderSingle::FileReaderSingle()
{
	_file = nullptr;
}

FileReaderSingle::~FileReaderSingle()
{
	if (_file!=nullptr)
	{
		fclose(_file);
		_file = nullptr;
	}
}

FileStatus FileReaderSingle::read(const std::string& filename, Buffer* buffer)
{
	size_t size = 0;
	auto status=openFile(filename, _file, size);
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


FileReaderPack::FileReaderPack():
_file(nullptr),
_contentOffset(0)
{

}

FileReaderPack::~FileReaderPack()
{
	if (_file != nullptr)
	{
		fclose(_file);
		_file = nullptr;
	}
	_fileDic.clear();
}

FileStatus FileReaderPack::initByPackFile(const std::string& packname)
{
	size_t size = 0;
	auto status = openFile(packname, _file, size);
	if (status != FileStatus::Openend)
	{
		return status;
	}
	fseek(_file, 0, 0);
	unsigned int fileCount = 0;
	size_t lenSize = sizeof(unsigned int);
	int readsize = fread(&fileCount, lenSize, 1, _file);
	if (readsize < lenSize)
	{
		return FileStatus::NotInitialized;
	}
	unsigned long headSize = 0;
	long offset;
	unsigned int fileSize;
	unsigned int pathlen;
	const int MAX_LEN = 255;
	char pathInfo[MAX_LEN] = { 0 };
	for (int i = 0; i < fileCount; i++)
	{
		readsize = fread(&fileSize, lenSize, 1, _file);
		if (readsize < lenSize)
		{
			return FileStatus::NotInitialized;
		}
		offset += fileSize;
		
		readsize = fread(&pathlen, lenSize, 1, _file);
		if (readsize < lenSize || pathlen>MAX_LEN)
		{
			return FileStatus::NotInitialized;
		}
		//memset((void*)pathInfo, 0, MAX_LEN);
		readsize = fread(&pathInfo, pathlen, 1, _file);
		
		if (readsize < lenSize)
		{
			return FileStatus::NotInitialized;
		}
		headSize += (pathlen + lenSize);
		pathInfo[pathlen] = '\0';
		FilePackInfo info;
		info.offset = offset;
		info.size = fileSize;
		_fileDic[std::string(pathInfo)] = info;
	}
	for (auto itr = _fileDic.begin(); itr != _fileDic.end();itr++)
	{
		itr->second.offset += headSize;
	}
	return FileStatus::Openend;
}


FileStatus FileReaderPack::read(const std::string& filename, Buffer* buffer)
{
	//if (!_file)
	//{
	//	return FileStatus::OpenFailed;
	//}
	auto info = _fileDic.find(filename);
	if (info == _fileDic.cend())
	{
		return FileStatus::NotExists;
	}
	fseek(_file, (long)info->second.offset, 0);
	size_t readsize=fread(buffer->buffer(), 1, info->second.size, _file);
	if (readsize < info->second.size)
	{
		buffer->resize(readsize);
		return FileStatus::ReadFailed;
	}
	return FileStatus::Success;
}

