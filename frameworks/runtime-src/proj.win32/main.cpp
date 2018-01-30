// LIR.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "stdarg.h"
// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <Windows.h>
#include <shellapi.h>


#include "Debug.h"

#include "SimulatorWin.h"





int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	int count = 0;
	auto test = SimulatorWin::getInstance();
	auto result=test->run();
	//if (!result.empty())
	//{
	//	const char* str = result.c_str();
		//const char* name = typeid(test).name();
		//lir::log("%s -----> [%s]\n", name, result);
	//}
	return 0;
}

