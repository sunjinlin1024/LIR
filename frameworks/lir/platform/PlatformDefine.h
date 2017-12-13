
#ifndef __LIR_PLATFORM_DEFINE_H__
#define __LIR_PLATFORM_DEFINE_H__
/// @cond DO_NOT_SHOW

#include "platform/PlatformConfig.h"

#if LIR_TARGET_PLATFORM == LIR_PLATFORM_MAC
#include "platform/mac/PlatformDefine-mac.h"
#elif LIR_TARGET_PLATFORM == LIR_PLATFORM_IOS
#include "platform/ios/PlatformDefine-ios.h"
#elif LIR_TARGET_PLATFORM == LIR_PLATFORM_ANDROID
#include "platform/android/PlatformDefine-android.h"
#elif LIR_TARGET_PLATFORM == LIR_PLATFORM_WIN32
#include "platform/win32/PlatformDefine-win32.h"
#elif LIR_TARGET_PLATFORM == LIR_PLATFORM_WINRT
#include "platform/winrt/CCPlatformDefine-winrt.h"
#elif LIR_TARGET_PLATFORM == LIR_PLATFORM_LINUX
#include "platform/linux/CCPlatformDefine-linux.h"
#elif LIR_TARGET_PLATFORM == LIR_PLATFORM_TIZEN
#include "platform/tizen/CCPlatformDefine-tizen.h"
#endif

/// @endcond
#endif /* __LIR_PLATFORM_DEFINE_H__*/
