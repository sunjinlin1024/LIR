#include "AppDelegate.h"
//#include "scripting/lua-bindings/manual/CCLuaEngine.h"
//#include "audio/include/SimpleAudioEngine.h"
#include "lir.h"
//#include "scripting/lua-bindings/manual/lua_module_register.h"
//
//#if (CC_TARGET_PLATFORM != CC_PLATFORM_LINUX)
//#include "ide-support/CodeIDESupport.h"
//#endif
//
//#if (COCOS2D_DEBUG > 0) && (CC_CODE_IDE_DEBUG_SUPPORT > 0)
//#include "runtime/Runtime.h"
//#include "ide-support/RuntimeLuaImpl.h"
//#endif
//
//using namespace CocosDenshion;

USING_NS_LIR;
using namespace std;

//LUALIB_API int luaopen_socketevent(lua_State *L);
//extern "C" {
//	int luaopen_pack(lua_State *L);
//	LUALIB_API int luaopen_bit(lua_State *L);
//	LUALIB_API int luaopen_ameba(lua_State *L);
//	LUALIB_API int luaopen_lsqlite3(lua_State *L);
//}

AppDelegate::AppDelegate()
{
    
}

AppDelegate::~AppDelegate()
{
    //todo
//    SimpleAudioEngine::end();

#if (COCOS2D_DEBUG > 0) && (CC_CODE_IDE_DEBUG_SUPPORT > 0)
    // NOTE:Please don't remove this call if you want to debug with Cocos Code IDE
    RuntimeEngine::getInstance()->end();
#endif

}

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs()
{
    //todo
    // set OpenGL context attributes: red,green,blue,alpha,depth,stencil
//    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};
//
//    GLView::setGLContextAttrs(glContextAttrs);
}

// if you want to use the package manager to install more packages, 
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching()
{
    //todo
//	// set default FPS
//    Director::getInstance()->setAnimationInterval(1.0 / 30.0f);
//	Director::getInstance()->setProjection(cocos2d::DisplayLinkDirector::Projection::_2D);
//
//    // register lua module
//    auto engine = LuaEngine::getInstance();
//    ScriptEngineManager::getInstance()->setScriptEngine(engine);
//    lua_State* L = engine->getLuaStack()->getLuaState();
//    lua_module_register(L);
//
//	luaopen_pack(L);
//	luaopen_bit(L);
//	luaopen_socketevent(L);
//#if (CC_TARGET_PLATFORM != CC_PLATFORM_LINUX)
//	luaopen_lsqlite3(L);
//#endif
//
//    register_all_packages();
//
//    LuaStack* stack = engine->getLuaStack();
//	std::string teaKey = "doudizhuverkey";
//	std::string teaSign = "gamesign";
//	stack->setXXTEAKeyAndSign(teaKey.c_str(), teaKey.length(), teaSign.c_str(), teaSign.length());
//
//    //register custom function
//    //LuaStack* stack = engine->getLuaStack();
//    //register_custom_function(stack->getLuaState());
//
//#if (COCOS2D_DEBUG > 0) && (CC_CODE_IDE_DEBUG_SUPPORT > 0)
//    // NOTE:Please don't remove this call if you want to debug with Cocos Code IDE
//    auto runtimeEngine = RuntimeEngine::getInstance();
//    runtimeEngine->addRuntime(RuntimeLuaImpl::create(), kRuntimeEngineLua);
//    runtimeEngine->start();
//#else
//    if (engine->executeScriptFile("src/mainentry.lua"))
//    {
//        return false;
//    }
//#endif
    
//    LPQFile::testInit("res", "res.lpq");
    
    LPQFile::testRead("res.lpq");
    
//    auto fullPath=FileUtils::getInstance()->fullPathForFilename("res/Tutorial03.png");
//    SingleFileC c;
//    c.open(fullPath,"rb+");
//    size_t size=0;
//    c.getSize(size);
    return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground()
{
    //todo
//	Director::getInstance()->stopAnimation();
//
//	SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
//    
//	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(EVENT_COME_TO_BACKGROUND);
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    //todo
//	Director::getInstance()->startAnimation();
//
//	SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
//    SimpleAudioEngine::getInstance()->resumeAllEffects();
//
//	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(EVENT_COME_TO_FOREGROUND);
}

