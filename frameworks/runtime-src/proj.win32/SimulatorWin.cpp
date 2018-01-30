
//#pragma comment(lib, "comctl32.lib")
//#pragma comment(linker, "\"/manifestdependency:type='Win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='X86' publicKeyToken='6595b64144ccf1df' language='*'\"")


#include "SimulatorWin.h"

#include "stdafx.h"
//#include <io.h>
//#include <stdlib.h>
//#include <malloc.h>
//#include <stdio.h>
//#include <fcntl.h>
//#include <Commdlg.h>
//#include <Shlobj.h>
//#include <winnls.h>
//#include <shobjidl.h>
//#include <objbase.h>
//#include <objidl.h>
//#include <shlguid.h>
//#include <shellapi.h>

#include "lir.h"
#include "IAnimatedMesh.h"
#include "ISceneManager.h"
#include "IMeshSceneNode.h"

//#include "glfw3.h"
//#include "glfw3native.h"
//
//#include "scripting/lua-bindings/manual/CCLuaEngine.h"
//#include "AppEvent.h"
//#include "AppLang.h"
//#include "runtime/ConfigParser.h"
//#include "runtime/Runtime.h"
//
//#include "platform/win32/PlayerWin.h"
//#include "platform/win32/PlayerMenuServiceWin.h"

// define 1 to open console ui and setup windows system menu, 0 to disable
//#include "ide-support/CodeIDESupport.h"
//#if (CC_CODE_IDE_DEBUG_SUPPORT > 0)
//#define SIMULATOR_WITH_CONSOLE_AND_MENU 1
//#else
//#define SIMULATOR_WITH_CONSOLE_AND_MENU 1
//#endif



using namespace std;
using namespace irr;
USING_NS_LIR;


SimulatorWin *SimulatorWin::_instance = nullptr;

SimulatorWin *SimulatorWin::getInstance()
{
    if (!_instance)
    {
        _instance = new SimulatorWin();
    }
    return _instance;
}

SimulatorWin::SimulatorWin()
    //: _app(nullptr)
    //,_hwnd(NULL)
    //, _hwndConsole(NULL)
    //,
	:_writeDebugLogFile(nullptr)
{
}

SimulatorWin::~SimulatorWin()
{
    //CC_SAFE_DELETE(_app);
    if (_writeDebugLogFile)
    {
        fclose(_writeDebugLogFile);
    }
}


int SimulatorWin::run()
{
	//INITCOMMONCONTROLSEX InitCtrls;
	//InitCtrls.dwSize = sizeof(InitCtrls);
	//InitCtrls.dwICC = ICC_WIN95_CLASSES;
	//InitCommonControlsEx(&InitCtrls);

	IrrlichtDevice *device =
		createDevice(video::EDT_OPENGL, core::dimension2d<u32>(640, 480));

	if (device == 0)
		return 1;

	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();

	device->getFileSystem()->addFileArchive("map-20kdm2.pk3");

	
	//auto reader=device->getFileSystem()->createAndOpenFile("config/ItemsConfig.lua");
	//if (reader)
	//{
	//	auto size = reader->getSize();
	//	void* buff=malloc(size);
	//	reader->read(buff,reader->getSize());
	//}

	scene::IAnimatedMesh* mesh = smgr->getMesh("20kdm2.bsp");
	scene::ISceneNode* node = 0;
	if (mesh)
		node = smgr->addOctreeSceneNode(mesh->getMesh(0), 0, -1, 1024);
	if (node)
		node->setPosition(core::vector3df(-1300, -144, -1249));
	smgr->addCameraSceneNodeFPS();

	device->getCursorControl()->setVisible(false);

	int lastFPS = -1;

	while (device->run())
	{
		if (device->isWindowActive())
		{
			driver->beginScene(true, true, video::SColor(255, 200, 200, 200));
			smgr->drawAll();
			driver->endScene();

			int fps = driver->getFPS();

			if (lastFPS != fps)
			{
				core::stringw str = L"Irrlicht Engine - Quake 3 Map example [";
				str += driver->getName();
				str += "] FPS:";
				str += fps;

				device->setWindowCaption(str.c_str());
				lastFPS = fps;
			}
		}
		else
			device->yield();
	}

	/*
	In the end, delete the Irrlicht device.
	*/
	device->drop();
	return 0;
}