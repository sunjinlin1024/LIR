//#include "stdafx.h"
#include "test/LirTest.h"
#include "lir.h"

//#include "Debug.h"
USING_NS_LIR




class LPQTest :lir::test::LirTest
{
public:
	std::string test() {
		//FileUtils* fileUtils = new FileUtils();
		//fileUtils->init();
		//FileUtils::setDelegate(fileUtils);


		//Buffer buffer;
		//FileUtils::getInstance()->getContents("res.zip",&buffer);


		//std::string engineRoot("F:/LIR/");
		//std::string dir("res");

		//auto fileUtils = FileUtils::getInstance();
		//fileUtils->addSearchPath(engineRoot);
		////fileUtils->addSearchResolutionsOrder("");

		//LPQFile* pack = new LPQFile();
		//
		////pack->create("F:/LIR/frameworks/runtime-src/LIR/Debug.win32/res.lrp");

		//
		//
		//Buffer buffer;

		//LPFILE file;
		//size_t size = 0;

		//pack->openLPQ(engineRoot + "res.lpq", "wb+");

		//std::vector<std::string> list;

		//FileUtils::getInstance()->listFilesRecursively(engineRoot.append(dir), &list,1);


		//auto startTime = clock();
		//const char* curName;
		//int appendSize = list.size();
		//int oldCount = pack->getCount();
		//pack->resize(oldCount + appendSize);
		//for (auto itr = list.begin(); itr != list.end(); itr++)
		//{
		//	if (fileUtils->isFileExist(*itr))
		//	{
		//		curName = (*itr).c_str();	
		//		if (pack->fopen(curName, "rb+", file, size) != FileStatus::Success)
		//		{
		//			if (file)
		//			{
		//				fclose(file);
		//			}
		//			return 0;
		//		}
		//		buffer.resize(size);
		//		fseek(file, 0, 0);
		//		fread(buffer.buffer(), 1, size, file);
		//		fclose(file);
		//		pack->append(curName + engineRoot.size() + 1, buffer.buffer(), size, oldCount++);
		//	}
		//}
		//pack->flush();
		//std::cout << "cost time 1,  " << clock() - startTime << std::endl;

		//////pack = new FileHandlerPack();
		//pack->openByPack("D:/work/lir/frameworks/runtime-src/LIR/Debug.win32/res.lrp", "rb+");

		//count = 10000;

		//auto startTime = clock();
		//for (int k = 0; k < count; k++)
		//{
		//	auto status = pack->read(name, &buffer);
		//}
		//auto endTime = clock();
		//std::cout << "cost time 1,  " << endTime - startTime << std::endl;



		//std::string fullPath = std::string(dir).append(name);
		//startTime = clock();
		////LPFILE file;
		//size_t fileSize=0;
		//for (int k = 0; k < count; k++)
		//{
		//	pack->open(fullPath, "rb", file, fileSize);
		//	buffer.resize(fileSize);
		//	fread(buffer.buffer(), 1, size, file);
		//	fclose(file);
		//}
		//endTime = clock();
		//std::cout << "cost time 2, " << endTime - startTime << std::endl;


		//auto status = pack->read(name, &buffer);
		//if (status != FileStatus::Success){
		//	lir::log("cannot find %s \n", name);
		//}else{
		//	LPFILE nFile = fopen(std::string(dir2).append(name).c_str(), "wb+");
		//	fseek(nFile, 0, 0);
		//	fwrite(buffer.buffer(), buffer.size(), 1, nFile);
		//	fclose(nFile);
		//}

		//delete pack;
		//buffer.clear();
		return "";
	}
};
