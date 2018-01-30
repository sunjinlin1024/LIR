// LIR.cpp : 定义控制台应用程序的入口点。
//

//#include "stdafx.h"
#include "test/LirTest.h"

//#include "lir.h"
//#include "Debug.h"

using namespace irr;


#define LIR_CALLBACK_0(__selector__, ...) std::bind(&__selector__, ##__VA_ARGS__)

//static bool processing = false;

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

class FileSystemTest :lir::test::LirTest
{
public:
	std::string run() {
		//auto t = std::thread(LIR_CALLBACK_0(networkThread));
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
		return "";
	}
};
