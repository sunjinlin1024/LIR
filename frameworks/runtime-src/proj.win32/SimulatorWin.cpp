
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
#include "irrlicht.h"
#include "IAnimatedMesh.h"
#include "ISceneManager.h"
#include "IMeshSceneNode.h"
#include "driverChoice.h"
#include "CCameraSceneNode.h"

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


enum
{
	// I use this ISceneNode ID to indicate a scene node that is
	// not pickable by getSceneNodeAndCollisionPointFromRay()
	ID_IsNotPickable = 0,

	// I use this flag in ISceneNode IDs to indicate that the
	// scene node can be picked by ray selection.
	IDFlag_IsPickable = 1 << 0,

	// I use this flag in ISceneNode IDs to indicate that the
	// scene node can be highlighted.  In this example, the
	// homonids can be highlighted, but the level mesh can't.
	IDFlag_IsHighlightable = 1 << 1
};


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


	SEvent event;
	auto size=sizeof(event);

	IrrlichtDevice *device =
		createDevice(video::EDT_OPENGL, core::dimension2d<u32>(640, 480));

	if (device == 0)
		return 1;

	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();

	device->getFileSystem()->addFileArchive("map-20kdm2.pk3");

	scene::IAnimatedMesh* q3levelmesh = smgr->getMesh("20kdm2.bsp");
	scene::IMeshSceneNode* q3node = 0;

	// The Quake mesh is pickable, but doesn't get highlighted.
	if (q3levelmesh)
		q3node = smgr->addOctreeSceneNode(q3levelmesh->getMesh(0), 0, IDFlag_IsPickable);

	scene::ITriangleSelector* selector = 0;

	if (q3node)
	{
		q3node->setPosition(core::vector3df(-1350, -130, -1400));

		selector = smgr->createOctreeTriangleSelector(
			q3node->getMesh(), q3node, 128);
		q3node->setTriangleSelector(selector);
		// We're not done with this selector yet, so don't drop it.
	}
	

	// Set a jump speed of 3 units per second, which gives a fairly realistic jump
	// when used with the gravity of (0, -10, 0) in the collision response animator.
	scene::ICameraSceneNode* camera =
		smgr->addCameraSceneNodeFPS(0, 100.0f, .3f, ID_IsNotPickable, 0, 0, true, 3.f);
	camera->setPosition(core::vector3df(50, 50, -60));
	camera->setTarget(core::vector3df(-70, 30, -60));

	if (selector)
	{
		scene::ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(
			selector, camera, core::vector3df(30, 50, 30),
			core::vector3df(0, -10, 0), core::vector3df(0, 30, 0));
		selector->drop(); // As soon as we're done with the selector, drop it.
		camera->addAnimator(anim);
		anim->drop();  // And likewise, drop the animator when we're done referring to it.
	}

	// Now I create three animated characters which we can pick, a dynamic light for
	// lighting them, and a billboard for drawing where we found an intersection.

	// First, let's get rid of the mouse cursor.  We'll use a billboard to show
	// what we're looking at.
	device->getCursorControl()->setVisible(false);

	// Add the billboard.
	scene::IBillboardSceneNode * bill = smgr->addBillboardSceneNode();
	bill->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
	bill->setMaterialTexture(0, driver->getTexture("particle.bmp"));
	bill->setMaterialFlag(video::EMF_LIGHTING, false);
	bill->setMaterialFlag(video::EMF_ZBUFFER, false);
	bill->setSize(core::dimension2d<f32>(50.0f, 50.0f));
	bill->setID(ID_IsNotPickable); // This ensures that we don't accidentally ray-pick it

								   /* Add 3 animated hominids, which we can pick using a ray-triangle intersection.
								   They all animate quite slowly, to make it easier to see that accurate triangle
								   selection is being performed. */
	scene::IAnimatedMeshSceneNode* node = 0;
	video::SMaterial material;

	// Add an MD2 node, which uses vertex-based animation.
	node = smgr->addAnimatedMeshSceneNode(smgr->getMesh("faerie.md2"),
		0, IDFlag_IsPickable | IDFlag_IsHighlightable);
	node->setPosition(core::vector3df(-90, -15, -140)); // Put its feet on the floor.
	node->setScale(core::vector3df(1.6f)); // Make it appear realistically scaled
	node->setMD2Animation(scene::EMAT_POINT);
	node->setAnimationSpeed(20.f);
	material.setTexture(0, driver->getTexture("faerie2.bmp"));
	material.Lighting = true;
	material.NormalizeNormals = true;
	node->getMaterial(0) = material;

	// Now create a triangle selector for it.  The selector will know that it
	// is associated with an animated node, and will update itself as necessary.
	selector = smgr->createTriangleSelector(node);
	node->setTriangleSelector(selector);
	selector->drop(); // We're done with this selector, so drop it now.

					  // And this B3D file uses skinned skeletal animation.
	node = smgr->addAnimatedMeshSceneNode(smgr->getMesh("ninja.b3d"),
		0, IDFlag_IsPickable | IDFlag_IsHighlightable);
	node->setScale(core::vector3df(10));
	node->setPosition(core::vector3df(-75, -66, -80));
	node->setRotation(core::vector3df(0, 90, 0));
	node->setAnimationSpeed(8.f);
	node->getMaterial(0).NormalizeNormals = true;
	node->getMaterial(0).Lighting = true;
	// Just do the same as we did above.
	selector = smgr->createTriangleSelector(node);
	node->setTriangleSelector(selector);
	selector->drop();

	// This X files uses skeletal animation, but without skinning.
	node = smgr->addAnimatedMeshSceneNode(smgr->getMesh("dwarf.x"),
		0, IDFlag_IsPickable | IDFlag_IsHighlightable);
	node->setPosition(core::vector3df(-70, -66, -30)); // Put its feet on the floor.
	node->setRotation(core::vector3df(0, -90, 0)); // And turn it towards the camera.
	node->setAnimationSpeed(20.f);
	node->getMaterial(0).Lighting = true;
	selector = smgr->createTriangleSelector(node);
	node->setTriangleSelector(selector);
	selector->drop();


	// And this mdl file uses skinned skeletal animation.
	node = smgr->addAnimatedMeshSceneNode(smgr->getMesh("yodan.mdl"),
		0, IDFlag_IsPickable | IDFlag_IsHighlightable);
	node->setPosition(core::vector3df(-90, -25, 20));
	node->setScale(core::vector3df(0.8f));
	node->getMaterial(0).Lighting = true;
	node->setAnimationSpeed(20.f);

	// Just do the same as we did above.
	selector = smgr->createTriangleSelector(node);
	node->setTriangleSelector(selector);
	selector->drop();

	material.setTexture(0, 0);
	material.Lighting = false;

	// Add a light, so that the unselected nodes aren't completely dark.
	scene::ILightSceneNode * light = smgr->addLightSceneNode(0, core::vector3df(-60, 100, 400),
		video::SColorf(1.0f, 1.0f, 1.0f, 1.0f), 600.0f);
	light->setID(ID_IsNotPickable); // Make it an invalid target for selection.

									// Remember which scene node is highlighted
	scene::ISceneNode* highlightedSceneNode = 0;
	scene::ISceneCollisionManager* collMan = smgr->getSceneCollisionManager();
	int lastFPS = -1;

	// draw the selection triangle only as wireframe
	material.Wireframe = true;

	while (device->run())
		if (device->isWindowActive())
		{
			driver->beginScene(true, true, 0);
			smgr->drawAll();

			// Unlight any currently highlighted scene node
			if (highlightedSceneNode)
			{
				highlightedSceneNode->setMaterialFlag(video::EMF_LIGHTING, true);
				highlightedSceneNode = 0;
			}

			core::line3d<f32> ray;
			ray.start = camera->getPosition();
			ray.end = ray.start + (camera->getTarget() - ray.start).normalize() * 1000.0f;

			// Tracks the current intersection point with the level or a mesh
			core::vector3df intersection;
			// Used to show with triangle has been hit
			core::triangle3df hitTriangle;

			scene::ISceneNode * selectedSceneNode =
				collMan->getSceneNodeAndCollisionPointFromRay(
					ray,
					intersection, // This will be the position of the collision
					hitTriangle, // This will be the triangle hit in the collision
					IDFlag_IsPickable, // This ensures that only nodes that we have
									   // set up to be pickable are considered
					0); // Check the entire scene (this is actually the implicit default)

						// If the ray hit anything, move the billboard to the collision position
						// and draw the triangle that was hit.
			if (selectedSceneNode)
			{
				bill->setPosition(intersection);

				// We need to reset the transform before doing our own rendering.
				driver->setTransform(video::ETS_WORLD, core::matrix4());
				driver->setMaterial(material);
				driver->draw3DTriangle(hitTriangle, video::SColor(0, 255, 0, 0));

				// We can check the flags for the scene node that was hit to see if it should be
				// highlighted. The animated nodes can be highlighted, but not the Quake level mesh
				if ((selectedSceneNode->getID() & IDFlag_IsHighlightable) == IDFlag_IsHighlightable)
				{
					highlightedSceneNode = selectedSceneNode;

					// Highlighting in this case means turning lighting OFF for this node,
					// which means that it will be drawn with full brightness.
					highlightedSceneNode->setMaterialFlag(video::EMF_LIGHTING, false);
				}
			}

			// We're all done drawing, so end the scene.
			driver->endScene();

			int fps = driver->getFPS();

			if (lastFPS != fps)
			{
				core::stringw str = L"Collision detection example - Irrlicht Engine [";
				str += driver->getName();
				str += "] FPS:";
				str += fps;

				device->setWindowCaption(str.c_str());
				lastFPS = fps;
			}
		}

	device->drop();
	return 0;
}