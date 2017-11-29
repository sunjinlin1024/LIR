#include "io/BaseFile.h"

//#include <stack>
#include <sys/stat.h>

#include "debug/Debug.h"

USING_NS_LIR

int BaseFile::fclose(LPFILE file)
{
#if LIR_TARGET_PLATFORM==LIR_PLATFORM_WIN32
	return ::fclose(file);
#elif LIR_TARGET_PLATFORM==LIR_PLATFORM_ANDROID
	return 1;
#endif
	return 1;
}


int BaseFile::fseek(LPFILE file, long _Offset, int _Origin)
{
#if LIR_TARGET_PLATFORM==LIR_PLATFORM_WIN32
	return ::fseek(file, _Offset, _Origin);
#elif LIR_TARGET_PLATFORM==LIR_PLATFORM_ANDROID
	return 1;
#endif
		return 1;
}
int BaseFile::fwrite(const void* buff, size_t size, int count, LPFILE file)
{
#if LIR_TARGET_PLATFORM==LIR_PLATFORM_WIN32
	return ::fwrite(buff, size, count, file);
#elif LIR_TARGET_PLATFORM==LIR_PLATFORM_ANDROID
	return 0;
#endif
	return 0;
}
int BaseFile::fread(void* buff, size_t size, int count, LPFILE file)
{
#if LIR_TARGET_PLATFORM==LIR_PLATFORM_WIN32
	return ::fread(buff, size, count, file);
#elif LIR_TARGET_PLATFORM==LIR_PLATFORM_ANDROID
	return 0;
#endif
	return 0;
}

FileStatus BaseFile::fopen(const std::string& fullPath, const char* mode, LPFILE &file, size_t& size)
{

#if LIR_TARGET_PLATFORM==LIR_PLATFORM_WIN32
	file = ::fopen(fullPath.c_str(), mode);
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
#elif LIR_TARGET_PLATFORM == LIR_PLATFORM_ANDROID
		
#endif
	return FileStatus::ReadFailed;
}



SingleFile::SingleFile()
{
	//_file = nullptr;
}

SingleFile::~SingleFile()
{
	//if (_file!=nullptr)
	//{
	//	fclose(_file);
	//	_file = nullptr;
	//}
}

FileStatus SingleFile::read(const std::string& filename, Buffer* &buffer)
{
	size_t size = 0;
	LPFILE _file=nullptr;
	auto status=this->fopen(filename,"rb", _file, size);
	if (status != FileStatus::Openend)
	{
		return status;
	}
	buffer->resize(size);
	size_t readsize = this->fread(buffer->buffer(), 1, size, _file);
	this->fclose(_file);
	_file = nullptr;

	if (readsize < size) {
		buffer->resize(readsize);
		return FileStatus::ReadFailed;
	}
	return FileStatus::Success;
}