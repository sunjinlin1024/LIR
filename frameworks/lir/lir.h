#ifndef __LIR_H__
#define __LIR_H__
/// @cond DO_NOT_SHOW
#include "platform/platformConfig.h"


#include "base/config.h"
#include "base/Macros.h"
#include "platform/StdC.h"


#include "io/LPQFile.h"
#include "io/FileUtils.h"

#if (LIR_TARGET_PLATFORM == LIR_PLATFORM_IOS)
#include "platform/ios/Application-ios.h"
//#include "platform/ios/CCGLViewImpl-ios.h"
//#include "platform/ios/CCGL-ios.h"
#include "platform/ios/StdC-ios.h"
#endif

NS_LIR_BEGIN

inline const char* getLIRVersion()
{
	return "";
};

NS_LIR_END

/// @endcond
#endif    // __LIR_H__
