
#ifndef __LIR_PLATFORM_CCSTDC_H__
#define __LIR_PLATFORM_CCSTDC_H__

#include "platform/PlatformConfig.h"

#if LIR_TARGET_PLATFORM ==LIR_PLATFORM_MAC
#include "platform/mac/StdC-mac.h"
#elif LIR_TARGET_PLATFORM ==LIR_PLATFORM_IOS
#include "platform/ios/StdC-ios.h"
#elif LIR_TARGET_PLATFORM ==LIR_PLATFORM_ANDROID
#include "platform/android/StdC-android.h"
#elif LIR_TARGET_PLATFORM ==LIR_PLATFORM_WIN32
#include "platform/win32/StdC-win32.h"
#elif LIR_TARGET_PLATFORM ==LIR_PLATFORM_WINRT
#include "platform/winrt/StdC.h"
#elif LIR_TARGET_PLATFORM ==LIR_PLATFORM_LINUX
#include "platform/linux/StdC-linux.h"
#elif LIR_TARGET_PLATFORM ==LIR_PLATFORM_TIZEN
#include "platform/tizen/StdC-tizen.h"
#endif


#endif /* __LIR_PLATFORM_CCSTDC_H__*/
