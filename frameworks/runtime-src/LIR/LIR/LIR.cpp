// LIR.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "stdarg.h"
#include <string>
#include <Windows.h>

#include "LDebug.h"


int _tmain(int argc, _TCHAR* argv[])
{
	int count = 0;
	while (true)
	{
		count++;
		lir::log("count %d ,test %s\n", count,"a");
		Sleep(1000);
		
	}
	return 0;
}

