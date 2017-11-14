// LIR.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "stdarg.h"
#include <string>
#include <thread>
#include <mutex>
#include <Windows.h>
#include <ctime>
#include <iostream>

#include "Debug.h"
#include "io/FileHandler.h"
#include "core/Buffer.h"


USING_NS_LIR

#define CC_CALLBACK_0(__selector__, ...) std::bind(&__selector__, ##__VA_ARGS__)



static bool processing = false;

//volatile int num(0);
//std::mutex mtx;


//void increase10kTimes()
//{
//	for (int k = 0; k < 100000; k++)
//	{
//		if (mtx.try_lock())
//		{
//			++num;
//			mtx.unlock();
//		}
//		else
//		{
//
//		}
//		
//		
//	}
//}

int _tmain(int argc, _TCHAR* argv[])
{
	int count = 0;

	//auto t = std::thread(CC_CALLBACK_0(networkThread));
	//
	//processing = true;
	//t.detach();

	////Sleep(1000);
	//while (processing)
	//{
	//	count++;
	//	lir::log("count %d \n", count);
	//	Sleep(1000);
	//}
	//lir::log("complete");

	//t.join();

	//std::thread threads[10];
	//for (int k = 0; k < 10; k++)
	//{
	//	threads[k] = std::thread(increase10kTimes);
	//}

	//for (int k = 0; k < 10; k++)
	//{
	//	threads[k].detach();
	//}
	//lir::log("test num %d \n", num);
		
	//FileUtils* fileUtils = new FileUtils();
	//fileUtils->init();
	//FileUtils::setDelegate(fileUtils);
	

	//Buffer buffer;
	//FileUtils::getInstance()->getContents("res.zip",&buffer);


	FileHandlerPack* pack = new FileHandlerPack();
	
	//pack->create("F:/LIR/frameworks/runtime-src/LIR/Debug.win32/res.lrp");

	
	
	Buffer buffer;

	FILE* file;
	size_t size = 0;

	char* dir = "F:/LIR/frameworks/runtime-src/LIR/Debug.win32/res/";
	char* dir2 = "F:/LIR/frameworks/runtime-src/LIR/Debug.win32/res2/";
	char* name = "b/playerinfo_up.png";

	//pack->openByPack("F:/LIR/frameworks/runtime-src/LIR/Debug.win32/res.lrp", "wb+");
	//pack->open(std::string(dir).append(name), "rb+", file, size);
	//buffer.resize(size);
	//fseek(file, 0, 0);
	//fread(buffer.buffer(),1,size,file);
	//pack->append(dir, name, buffer.buffer(), size);
	//pack->reset();

	//pack = new FileHandlerPack();
	pack->openByPack("F:/LIR/frameworks/runtime-src/LIR/Debug.win32/res.lrp", "rb+");

	count = 100000;

	auto startTime = clock();
	for (int k = 0; k < count; k++)
	{
		auto status = pack->read(name, &buffer);
	}
	auto endTime = clock();
	std::cout << "cost time 1,  " << endTime - startTime << std::endl;



	std::string fullPath = std::string(dir).append(name);
	startTime = clock();
	//FILE* file;
	size_t fileSize=0;
	for (int k = 0; k < count; k++)
	{
		pack->open(fullPath, "rb", file, fileSize);
		buffer.resize(fileSize);
		fread(buffer.buffer(), 1, size, file);
		fclose(file);
	}
	endTime = clock();
	std::cout << "cost time 2, " << endTime - startTime << std::endl;

	
	
	//if (status != FileStatus::Success){
	//	lir::log("cannot find %s \n", name);
	//}else{
	//	FILE* nFile = fopen(std::string(dir2).append(name).c_str(), "wb+");
	//	fseek(nFile, 0, 0);
	//	fwrite(buffer.buffer(), buffer.size(), 1, nFile);
	//	fclose(nFile);
	//}

	while (true){
		count++;
		//lir::log("count %d \n", count);
		//lir::log("test num %d \n", num);
		Sleep(1000);
	}
	return 0;
}

