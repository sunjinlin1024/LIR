#ifndef __LIR_PLATFORM_CONFIG_H__
#define __LIR_PLATFORM_CONFIG_H__

// define supported target platform macro which CC uses.
#define LIR_PLATFORM_UNKNOWN            0
#define LIR_PLATFORM_IOS                1
#define LIR_PLATFORM_ANDROID            2
#define LIR_PLATFORM_WIN32              3
#define LIR_PLATFORM_MARMALADE          4
#define LIR_PLATFORM_LINUX              5
#define LIR_PLATFORM_BADA               6
#define LIR_PLATFORM_BLACKBERRY         7
#define LIR_PLATFORM_MAC                8
#define LIR_PLATFORM_NACL               9
#define LIR_PLATFORM_EMSCRIPTEN        10
#define LIR_PLATFORM_TIZEN             11
#define LIR_PLATFORM_QT5               12
#define LIR_PLATFORM_WINRT             13

// Determine target platform by compile environment macro.
#define LIR_TARGET_PLATFORM             LIR_PLATFORM_UNKNOWN

// Apple: Mac and iOS
#if defined(__APPLE__) && !defined(ANDROID) // exclude android for binding generator.
    #include <TargetConditionals.h>
    #if TARGET_OS_IPHONE // TARGET_OS_IPHONE includes TARGET_OS_IOS TARGET_OS_TV and TARGET_OS_WATCH. see TargetConditionals.h
        #undef  LIR_TARGET_PLATFORM
        #define LIR_TARGET_PLATFORM         LIR_PLATFORM_IOS
    #elif TARGET_OS_MAC
        #undef  LIR_TARGET_PLATFORM
        #define LIR_TARGET_PLATFORM         LIR_PLATFORM_MAC
    #endif
#endif

// android
#if defined(ANDROID)
    #undef  LIR_TARGET_PLATFORM
    #define LIR_TARGET_PLATFORM         LIR_PLATFORM_ANDROID
#endif

// win32
#if defined(WIN32)
    #undef  LIR_TARGET_PLATFORM
    #define LIR_TARGET_PLATFORM         LIR_PLATFORM_WIN32
#endif

// linux
#if defined(LINUX) && !defined(__APPLE__)
    #undef  LIR_TARGET_PLATFORM
    #define LIR_TARGET_PLATFORM         LIR_PLATFORM_LINUX
#endif

// marmalade
#if defined(MARMALADE)
#undef  LIR_TARGET_PLATFORM
#define LIR_TARGET_PLATFORM         LIR_PLATFORM_MARMALADE
#endif

// bada
#if defined(SHP)
#undef  LIR_TARGET_PLATFORM
#define LIR_TARGET_PLATFORM         LIR_PLATFORM_BADA
#endif

// qnx
#if defined(__QNX__)
    #undef  LIR_TARGET_PLATFORM
    #define LIR_TARGET_PLATFORM     LIR_PLATFORM_BLACKBERRY
#endif

// native client
#if defined(__native_client__)
    #undef  LIR_TARGET_PLATFORM
    #define LIR_TARGET_PLATFORM     LIR_PLATFORM_NACL
#endif

// Emscripten
#if defined(EMSCRIPTEN)
    #undef  LIR_TARGET_PLATFORM
    #define LIR_TARGET_PLATFORM     LIR_PLATFORM_EMSCRIPTEN
#endif

// tizen
#if defined(TIZEN)
    #undef  LIR_TARGET_PLATFORM
    #define LIR_TARGET_PLATFORM     LIR_PLATFORM_TIZEN
#endif

// qt5
#if defined(LIR_TARGET_QT5)
    #undef  LIR_TARGET_PLATFORM
    #define LIR_TARGET_PLATFORM     LIR_PLATFORM_QT5
#endif

// WinRT (Windows 8.1 Store/Phone App)
#if defined(WINRT)
    #undef  LIR_TARGET_PLATFORM
    #define LIR_TARGET_PLATFORM          LIR_PLATFORM_WINRT
#endif

//////////////////////////////////////////////////////////////////////////
// post configure
//////////////////////////////////////////////////////////////////////////

// check user set platform
#if ! LIR_TARGET_PLATFORM
    #error  "Cannot recognize the target platform; are you targeting an unsupported platform?"
#endif

#if (LIR_TARGET_PLATFORM == LIR_PLATFORM_WIN32)
#ifndef __MINGW32__
#pragma warning (disable:4127)
#endif
#endif  // LIR_PLATFORM_WIN32

#if ((LIR_TARGET_PLATFORM == LIR_PLATFORM_ANDROID) || (LIR_TARGET_PLATFORM == LIR_PLATFORM_IOS) || (LIR_TARGET_PLATFORM == LIR_PLATFORM_TIZEN))
    #define LIR_PLATFORM_MOBILE
#else
    #define LIR_PLATFORM_PC
#endif

/// @endcond
#endif  // __BASE_LIR_PLATFORM_CONFIG_H__
