#ifndef __LIR_PLATFORM_DEFINE_IOS_H__
#define __LIR_PLATFORM_DEFINE_IOS_H__

#include "platform/PlatformConfig.h"
#if LIR_TARGET_PLATFORM == LIR_PLATFORM_IOS

#include <assert.h>

#define LIR_DLL 

#define L_ASSERT(cond) assert(cond)


#define LIR_UNUSED_PARAM(unusedparam) (void)unusedparam

/* Define NULL pointer value */
#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif


#endif // LIR_PLATFORM_IOS

#endif /* __LIR_PLATFORM_DEFINE_IOS_H__*/
