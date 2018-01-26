// LIR.cpp : 定义控制台应用程序的入口点。
//

//#include "stdafx.h"
#include "test/LirTest.h"

//#include "lir.h"
//#include "Debug.h"

#include "CFileSystem.h"
using namespace irr;




class FileSystemTest :lir::test::LirTest
{
public:
	std::string test() {
		io::CFileSystem system;
		//system.addFileArchive("res.lpq", false, true, io::EFAT_LPQ);

		system.addFileArchive("res/", false, false, io::EFAT_FOLDER);
		system.addFileArchive("src/", false, false, io::EFAT_FOLDER);

		auto reader = system.createAndOpenFile("aaa.txt");//config/HallFuncConfig.lua");
		auto size = reader->getSize();
		char* buff = new char[size];
		reader->read(buff, size);
		auto file = fopen("test.lua", "wb+");
		fwrite(buff, size, 1, file);
		fclose(file);
		delete buff;

		return "a";
	}
};
