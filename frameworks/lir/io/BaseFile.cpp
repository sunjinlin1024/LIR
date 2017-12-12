#include "io/BaseFile.h"

//#include <stack>
#include <sys/stat.h>

#include "debug/Debug.h"

USING_NS_LIR

BaseFile::BaseFile()

{

}

BaseFile::~BaseFile()
{
	
}


SingleFileC::SingleFileC()
:_file(nullptr)
{
}

SingleFileC::~SingleFileC()
{
	if (_file)
	{
		this->close();
		_file = nullptr;
	}
}

FileStatus SingleFileC::open(const std::string& fullPath, const char* mode)
{
	_file = ::fopen(fullPath.c_str(), mode);
	if (!_file)
		return FileStatus::OpenFailed;
	return FileStatus::Success;
}

FileStatus SingleFileC::close()
{
	if (_file){
	 ::fclose(_file);
	}
	return FileStatus::Success;
}

FileStatus SingleFileC::seek(const long& offset,const int& origin)
{
	if (fseek(_file, offset, origin) == 0)
	{
		return FileStatus::Success;
	}
	else{
		return FileStatus::SeekFailed;
	}
}
FileStatus SingleFileC::write(const void* buff, const size_t& size, const size_t& count)
{
	if (fwrite(buff, size, count, _file) / size == count){
		return FileStatus::Success;
	}
	else{
		return FileStatus::WriteFailed;
	}
}
FileStatus SingleFileC::read(void* buff, const size_t& size, const size_t& count)
{
	if (fread(buff, size, count, _file) == count){
		return FileStatus::Success;
	}
	else{
		return FileStatus::ReadFailed;
	}
}




void SingleFileC::getSize(size_t& size)
{
	if (!_file){
		size = 0;
		return;
	}
#if LIR_TARGET_PLATFORM ==LIR_PLATFORM_WIN32 
#if defined(_MSC_VER)
	auto descriptor = _fileno(_file);
#else
	auto descriptor = fileno(fp);
#endif
	struct stat statBuf;
	if (fstat(descriptor, &statBuf) == -1) {
		size = 0;
		return;
	}
	size = statBuf.st_size;
#else//if LIR_TARGET_PLATFORM==LIR_PLATFORM_ANDROID
	size = ::ftell(_file);
#endif
}




FileStatus SingleFileC::read(Buffer* &buffer)
{
	size_t size=0;
	this->getSize(size);
	buffer->resize(size);
	if (size <= 0){
		return FileStatus::NotExists;
	}
	return this->read(buffer->buffer(), size, 1);
}