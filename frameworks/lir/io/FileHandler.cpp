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