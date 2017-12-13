#ifndef __LIR_PLATFORM_APPLICATION_H__
#define __LIR_PLATFORM_APPLICATION_H__
/// @cond DO_NOT_SHOW

#include "platform/PlatformConfig.h"

#if LIR_TARGET_PLATFORM == LIR_PLATFORM_MAC
#include "platform/mac/Application-mac.h"
#elif LIR_TARGET_PLATFORM == LIR_PLATFORM_IOS
#include "platform/ios/Application-ios.h"
#elif LIR_TARGET_PLATFORM == LIR_PLATFORM_ANDROID
#include "platform/android/Application-android.h"
#elif LIR_TARGET_PLATFORM == LIR_PLATFORM_WIN32
#include "platform/win32/Application-win32.h"
#elif LIR_TARGET_PLATFORM == LIR_PLATFORM_WINRT
#include "platform/winrt/Application.h"
#elif LIR_TARGET_PLATFORM == LIR_PLATFORM_LINUX
#include "platform/linux/Application-linux.h"
#elif LIR_TARGET_PLATFORM == LIR_PLATFORM_TIZEN
#include "platform/tizen/Application-tizen.h"
#endif

/// @endcond
#endif /* __LIR_PLATFORM_APPLICATION_H__*/
