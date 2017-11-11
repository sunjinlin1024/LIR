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


FileReader::FileStatus FileReader::openFile(const std::string& filename, FILE* file,size_t& size )
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
	return FileReader::Openend;
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


FileReader::FileStatus FileReaderSingle::read(const std::string& filename, Buffer* buffer)
{
	size_t size = 0;
	auto status=this->openFile(filename, _file, size);
	if (status != FileReader::FileStatus::Openend)
	{
		return status;
	}
	buffer->resize(size);
	size_t readsize = fread(buffer->buffer(), 1, size, _file);
	fclose(_file);
	_file = nullptr;

	if (readsize < size) {
		buffer->resize(readsize);
		return FileReader::FileStatus::ReadFailed;
	}

	return FileReader::FileStatus::SUCCESS;
}


FileReader::FileStatus FileReaderPack::initByPack(const std::string& packname)
{
	_file = fopen(packname.c_str(), "rb");
}

