#ifndef __LIR_STD_C_IOS_H__
#define __LIR_STD_C_IOS_H__

#include "platform/PlatformConfig.h"
#if LIR_TARGET_PLATFORM == LIR_PLATFORM_IOS

#include "platform/PlatformMacros.h"
#include <float.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <stdint.h>

#ifndef MIN
#define MIN(x,y) (((x) > (y)) ? (y) : (x))
#endif  // MIN

#ifndef MAX
#define MAX(x,y) (((x) < (y)) ? (y) : (x))
#endif  // MAX

#endif // LIR_PLATFORM_IOS
#endif  // __LIR_STD_C_IOS_H__
