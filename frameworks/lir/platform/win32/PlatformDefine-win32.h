#ifndef __PLATFORMDEFINE_H__
#define __PLATFORMDEFINE_H__

#include "platform/PlatformConfig.h"
#if LIR_TARGET_PLATFORM == LIR_PLATFORM_WIN32

#ifdef __MINGW32__
#include <string.h>
#endif

#if defined(LIR_STATIC)
    #define LIR_DLL
#else
#if defined(_USRDLL)
    #define LIR_DLL     __declspec(dllexport)
#else         /* use a DLL library */
    #define LIR_DLL     __declspec(dllimport)
#endif
#endif

#include <assert.h>

#if LIR_DISABLE_ASSERT > 0
#define LIR_ASSERT(cond)
#else
#define LIR_ASSERT(cond)    assert(cond)
#endif
#define LIR_UNUSED_PARAM(unusedparam) (void)unusedparam

/* Define NULL pointer value */
#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif

#if _MSC_VER > 1800
//#pragma comment(lib,"libpng-2015.lib")
//#pragma comment(lib,"libjpeg-2015.lib")
//#pragma comment(lib,"libtiff-2015.lib")
//#pragma comment(lib,"glfw3-2015.lib")
//#pragma comment(lib,"libchipmunk-2015.lib")
#else
//#pragma comment(lib,"libpng.lib")
//#pragma comment(lib,"libjpeg.lib")
//#pragma comment(lib,"libtiff.lib")
//#pragma comment(lib,"glfw3.lib")
//#pragma comment(lib,"libchipmunk.lib")
#endif

#endif //s LIR_TARGET_PLATFORM == LIR_PLATFORM_WIN32

#ifdef __MINGW32__
#include <string.h>
#endif

#if defined(CC_STATIC)
#define LIR_DLL
#else
#if defined(_USRDLL)
#define LIR_DLL     __declspec(dllexport)
#else         /* use a DLL library */
#define LIR_DLL     //__declspec(dllimport)
#endif
#endif

#endif /* __PLATFORMDEFINE_H__*/
