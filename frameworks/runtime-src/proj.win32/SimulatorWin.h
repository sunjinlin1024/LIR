#pragma once

#include "stdafx.h"
//#include "Resource.h"
//#include "lir.h"
//#include "AppDelegate.h"
//#include "ProjectConfig/ProjectConfig.h"
//#include "ProjectConfig/SimulatorConfig.h"

class SimulatorWin
{
public:
    static SimulatorWin *getInstance();
    virtual ~SimulatorWin();
    int run();
protected:
    SimulatorWin();

    static SimulatorWin *_instance;
    //ProjectConfig _project;
    //HWND _hwnd;
    //HWND _hwndConsole;
    //AppDelegate *_app;
    FILE *_writeDebugLogFile;

    //static LRESULT CALLBACK windowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

