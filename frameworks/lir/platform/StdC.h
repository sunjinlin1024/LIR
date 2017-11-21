
#ifndef __PLATFORM_CCSTDC_H__
#define __PLATFORM_CCSTDC_H__

#include "platform/PlatformConfig.h"

#if LIR_TARGET_PLATFORM ==LIR_PLATFORM_MAC
#include "platform/mac/CCStdC-mac.h"
#elif LIR_TARGET_PLATFORM ==LIR_PLATFORM_IOS
#include "platform/ios/CCStdC-ios.h"
#elif LIR_TARGET_PLATFORM ==LIR_PLATFORM_ANDROID
#include "platform/android/CCStdC-android.h"
#elif LIR_TARGET_PLATFORM ==LIR_PLATFORM_WIN32
#include "platform/win32/StdC-win32.h"
#elif LIR_TARGET_PLATFORM ==LIR_PLATFORM_WINRT
#include "platform/winrt/CCStdC.h"
#elif LIR_TARGET_PLATFORM ==LIR_PLATFORM_LINUX
#include "platform/linux/CCStdC-linux.h"
#elif LIR_TARGET_PLATFORM ==LIR_PLATFORM_TIZEN
#include "platform/tizen/CCStdC-tizen.h"
#endif


#endif /* __PLATFORM_CCSTDC_H__*/
