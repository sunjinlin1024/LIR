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

#include "test/FileSystemTest.h"

USING_NS_LIR



int _tmain(int argc, _TCHAR* argv[])
{
	int count = 0;
	FileSystemTest test;
	auto result=test.test();
	if (!result.empty())
	{
		//int a = 1;
		const char* str = result.c_str();
		const char* name = typeid(test).name();
		lir::log("[%s] test result:[%s]\n", name, str);
	}
	while (true){
		//count++;
		//lir::log("\ncount %d", count);
		//lir::log("test num %d \n", num);
		Sleep(1000);
	}
	return 0;
}

