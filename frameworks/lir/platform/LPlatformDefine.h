
#ifndef __PLATFORM_LPLATFORMDEFINE_H__
#define __PLATFORM_LPLATFORMDEFINE_H__
/// @cond DO_NOT_SHOW

#include "platform/LPlatformConfig.h"

#if LIR_TARGET_PLATFORM == LIR_PLATFORM_MAC
#include "platform/mac/CCPlatformDefine-mac.h"
#elif LIR_TARGET_PLATFORM == LIR_PLATFORM_IOS
#include "platform/ios/CCPlatformDefine-ios.h"
#elif LIR_TARGET_PLATFORM == LIR_PLATFORM_ANDROID
#include "platform/android/CCPlatformDefine-android.h"
#elif LIR_TARGET_PLATFORM == LIR_PLATFORM_WIN32
#include "platform/win32/LPlatformDefine-win32.h"
#elif LIR_TARGET_PLATFORM == LIR_PLATFORM_WINRT
#include "platform/winrt/CCPlatformDefine-winrt.h"
#elif LIR_TARGET_PLATFORM == LIR_PLATFORM_LINUX
#include "platform/linux/CCPlatformDefine-linux.h"
#elif LIR_TARGET_PLATFORM == LIR_PLATFORM_TIZEN
#include "platform/tizen/CCPlatformDefine-tizen.h"
#endif

/// @endcond
#endif /* __PLATFORM_LPLATFORMDEFINE_H__*/
