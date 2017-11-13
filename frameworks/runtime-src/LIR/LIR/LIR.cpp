// LIR.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "stdarg.h"
#include <string>
#include <thread>
#include <mutex>
#include <Windows.h>

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
	pack->openByPack("./res.lrp","wb+");
	

	while (true){
		count++;
		//lir::log("count %d \n", count);
		//lir::log("test num %d \n", num);
		Sleep(1000);
	}
	return 0;
}

