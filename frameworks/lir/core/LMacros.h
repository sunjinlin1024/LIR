
#ifndef __BASE_LMACROS_H__
#define __BASE_LMACROS_H__

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

//#include "core/LConsole.h"
#include "platform/LStdC.h"

#ifndef LASSERT
#if LIR_DEBUG > 0
   // #if LIR_ENABLE_SCRIPT_BINDING
   // extern bool LIR_DLL lir_assert_script_compatible(const char *msg);
   // #define LASSERT(cond, msg) do {                              \
   //       if (!(cond)) {                                          \
			//if (!lir_assert_script_compatible(msg) && strlen(msg)) \
   //           lir::log("Assert failed: %s", msg);             \
   //         L_ASSERT(cond);                                      \
   //       } \
   //     } while (0)
   // #else
   // #define LASSERT(cond, msg) LIR_ASSERT(cond)
   // #endif
	#define LASSERT(cond,msg) if(!cond)\
		lir::log("Assert failed: %s", msg);\
	}
#else
    #define LASSERT(cond, msg)
#endif

#define GP_ASSERT(cond) LASSERT(cond, "")

// FIXME:: Backward compatible
#define L_ASSERT LASSERT
#endif  // LASSERT

#include "core/LConfig.h"

/** @def LIR_SWAP
simple macro that swaps 2 variables
 @deprecated use std::swap() instead
*/
#define LIR_SWAP(x, y, type)    \
{    type temp = (x);        \
    x = y; y = temp;        \
}

#include "core/LRandom.h"

/** @def CCRANDOM_MINUS1_1
 returns a random float between -1 and 1
 */
#define CCRANDOM_MINUS1_1() cocos2d::rand_minus1_1()

/** @def CCRANDOM_0_1
 returns a random float between 0 and 1
 */
#define CCRANDOM_0_1() cocos2d::rand_0_1()

/** @def LIR_DEGREES_TO_RADIANS
 converts degrees to radians
 */
#define LIR_DEGREES_TO_RADIANS(__ANGLE__) ((__ANGLE__) * 0.01745329252f) // PI / 180

/** @def LIR_RADIANS_TO_DEGREES
 converts radians to degrees
 */
#define LIR_RADIANS_TO_DEGREES(__ANGLE__) ((__ANGLE__) * 57.29577951f) // PI * 180

#define LIR_REPEAT_FOREVER (UINT_MAX -1)
#define kRepeatForever LIR_REPEAT_FOREVER

/** @def LIR_BLEND_SRC
default gl blend src function. Compatible with premultiplied alpha images.
*/
#define LIR_BLEND_SRC GL_ONE
#define LIR_BLEND_DST GL_ONE_MINUS_SRC_ALPHA


/** @def LIR_NODE_DRAW_SETUP
 Helpful macro that setups the GL server state, the correct GL program and sets the Model View Projection matrix
 @since v2.0
 */
#define LIR_NODE_DRAW_SETUP() \
do { \
    LASSERT(getGLProgram(), "No shader program set for this node"); \
    { \
        getGLProgram()->use(); \
        getGLProgram()->setUniformsForBuiltins(_modelViewTransform); \
    } \
} while(0)


 /** @def LIR_DIRECTOR_END
  Stops and removes the director from memory.
  Removes the GLView from its parent

  @since v0.99.4
  */
#define LIR_DIRECTOR_END()                                       \
do {                                                            \
    Director *__director = cocos2d::Director::getInstance();             \
    __director->end();                                          \
} while(0)

/** @def LIR_CONTENT_SCALE_FACTOR
On Mac it returns 1;
On iPhone it returns 2 if RetinaDisplay is On. Otherwise it returns 1
*/
#define LIR_CONTENT_SCALE_FACTOR() cocos2d::Director::getInstance()->getContentScaleFactor()

/****************************/
/** RETINA DISPLAY ENABLED **/
/****************************/

/** @def LIR_RECT_PIXELS_TO_POINTS
 Converts a rect in pixels to points
 */
#define LIR_RECT_PIXELS_TO_POINTS(__rect_in_pixels__)                                                                        \
    Rect( (__rect_in_pixels__).origin.x / LIR_CONTENT_SCALE_FACTOR(), (__rect_in_pixels__).origin.y / LIR_CONTENT_SCALE_FACTOR(),    \
            (__rect_in_pixels__).size.width / LIR_CONTENT_SCALE_FACTOR(), (__rect_in_pixels__).size.height / LIR_CONTENT_SCALE_FACTOR() )

/** @def LIR_RECT_POINTS_TO_PIXELS
 Converts a rect in points to pixels
 */
#define LIR_RECT_POINTS_TO_PIXELS(__rect_in_points_points__)                                                                        \
    Rect( (__rect_in_points_points__).origin.x * LIR_CONTENT_SCALE_FACTOR(), (__rect_in_points_points__).origin.y * LIR_CONTENT_SCALE_FACTOR(),    \
            (__rect_in_points_points__).size.width * LIR_CONTENT_SCALE_FACTOR(), (__rect_in_points_points__).size.height * LIR_CONTENT_SCALE_FACTOR() )

/** @def LIR_POINT_PIXELS_TO_POINTS
 Converts a rect in pixels to points
 */
#define LIR_POINT_PIXELS_TO_POINTS(__pixels__)                                                                        \
Vec2( (__pixels__).x / LIR_CONTENT_SCALE_FACTOR(), (__pixels__).y / LIR_CONTENT_SCALE_FACTOR())

/** @def LIR_POINT_POINTS_TO_PIXELS
 Converts a rect in points to pixels
 */
#define LIR_POINT_POINTS_TO_PIXELS(__points__)                                                                        \
Vec2( (__points__).x * LIR_CONTENT_SCALE_FACTOR(), (__points__).y * LIR_CONTENT_SCALE_FACTOR())

/** @def LIR_POINT_PIXELS_TO_POINTS
 Converts a rect in pixels to points
 */
#define LIR_SIZE_PIXELS_TO_POINTS(__size_in_pixels__)                                                                        \
Size( (__size_in_pixels__).width / LIR_CONTENT_SCALE_FACTOR(), (__size_in_pixels__).height / LIR_CONTENT_SCALE_FACTOR())

/** @def LIR_POINT_POINTS_TO_PIXELS
 Converts a rect in points to pixels
 */
#define LIR_SIZE_POINTS_TO_PIXELS(__size_in_points__)                                                                        \
Size( (__size_in_points__).width * LIR_CONTENT_SCALE_FACTOR(), (__size_in_points__).height * LIR_CONTENT_SCALE_FACTOR())


#ifndef FLT_EPSILON
#define FLT_EPSILON     1.192092896e-07F
#endif // FLT_EPSILON

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
            TypeName(const TypeName&);\
            void operator=(const TypeName&)

/**
Helper macros which converts 4-byte little/big endian 
integral number to the machine native number representation
 
It should work same as apples CFSwapInt32LittleToHost(..)
*/

/// when define returns true it means that our architecture uses big endian
#define LIR_HOST_IS_BIG_ENDIAN (bool)(*(unsigned short *)"\0\xff" < 0x100) 
#define LIR_SWAP32(i)  ((i & 0x000000ff) << 24 | (i & 0x0000ff00) << 8 | (i & 0x00ff0000) >> 8 | (i & 0xff000000) >> 24)
#define LIR_SWAP16(i)  ((i & 0x00ff) << 8 | (i &0xff00) >> 8)   
#define LIR_SWAP_INT32_LITTLE_TO_HOST(i) ((LIR_HOST_IS_BIG_ENDIAN == true)? LIR_SWAP32(i) : (i) )
#define LIR_SWAP_INT16_LITTLE_TO_HOST(i) ((LIR_HOST_IS_BIG_ENDIAN == true)? LIR_SWAP16(i) : (i) )
#define LIR_SWAP_INT32_BIG_TO_HOST(i)    ((LIR_HOST_IS_BIG_ENDIAN == true)? (i) : LIR_SWAP32(i) )
#define LIR_SWAP_INT16_BIG_TO_HOST(i)    ((LIR_HOST_IS_BIG_ENDIAN == true)? (i):  LIR_SWAP16(i) )

/**********************/
/** Profiling Macros **/
/**********************/
#if LIR_ENABLE_PROFILERS

#define LIR_PROFILER_DISPLAY_TIMERS() NS_CC::Profiler::getInstance()->displayTimers()
#define LIR_PROFILER_PURGE_ALL() NS_CC::Profiler::getInstance()->releaseAllTimers()

#define LIR_PROFILER_START(__name__) NS_CC::ProfilingBeginTimingBlock(__name__)
#define LIR_PROFILER_STOP(__name__) NS_CC::ProfilingEndTimingBlock(__name__)
#define LIR_PROFILER_RESET(__name__) NS_CC::ProfilingResetTimingBlock(__name__)

#define LIR_PROFILER_START_CATEGORY(__cat__, __name__) do{ if(__cat__) NS_CC::ProfilingBeginTimingBlock(__name__); } while(0)
#define LIR_PROFILER_STOP_CATEGORY(__cat__, __name__) do{ if(__cat__) NS_CC::ProfilingEndTimingBlock(__name__); } while(0)
#define LIR_PROFILER_RESET_CATEGORY(__cat__, __name__) do{ if(__cat__) NS_CC::ProfilingResetTimingBlock(__name__); } while(0)

#define LIR_PROFILER_START_INSTANCE(__id__, __name__) do{ NS_CC::ProfilingBeginTimingBlock( NS_CC::String::createWithFormat("%08X - %s", __id__, __name__)->getCString() ); } while(0)
#define LIR_PROFILER_STOP_INSTANCE(__id__, __name__) do{ NS_CC::ProfilingEndTimingBlock(    NS_CC::String::createWithFormat("%08X - %s", __id__, __name__)->getCString() ); } while(0)
#define LIR_PROFILER_RESET_INSTANCE(__id__, __name__) do{ NS_CC::ProfilingResetTimingBlock( NS_CC::String::createWithFormat("%08X - %s", __id__, __name__)->getCString() ); } while(0)


#else

#define LIR_PROFILER_DISPLAY_TIMERS() do {} while (0)
#define LIR_PROFILER_PURGE_ALL() do {} while (0)

#define LIR_PROFILER_START(__name__)  do {} while (0)
#define LIR_PROFILER_STOP(__name__) do {} while (0)
#define LIR_PROFILER_RESET(__name__) do {} while (0)

#define LIR_PROFILER_START_CATEGORY(__cat__, __name__) do {} while(0)
#define LIR_PROFILER_STOP_CATEGORY(__cat__, __name__) do {} while(0)
#define LIR_PROFILER_RESET_CATEGORY(__cat__, __name__) do {} while(0)

#define LIR_PROFILER_START_INSTANCE(__id__, __name__) do {} while(0)
#define LIR_PROFILER_STOP_INSTANCE(__id__, __name__) do {} while(0)
#define LIR_PROFILER_RESET_INSTANCE(__id__, __name__) do {} while(0)

#endif

#if !defined(COCOS2D_DEBUG) || COCOS2D_DEBUG == 0
#define CHECK_GL_ERROR_DEBUG()
#else
#define CHECK_GL_ERROR_DEBUG() \
    do { \
        GLenum __error = glGetError(); \
        if(__error) { \
            cocos2d::log("OpenGL error 0x%04X in %s %s %d\n", __error, __FILE__, __FUNCTION__, __LINE__); \
        } \
    } while (false)
#endif

/**
 * GL assertion that can be used for any OpenGL function call.
 *
 * This macro will assert if an error is detected when executing
 * the specified GL code. This macro will do nothing in release
 * mode and is therefore safe to use for realtime/per-frame GL
 * function calls.
 */
#if defined(NDEBUG) || (defined(__APPLE__) && !defined(DEBUG))
#define LIR_GL_ASSERT( gl_code ) gl_code
#else
#define LIR_GL_ASSERT( gl_code ) do \
{ \
gl_code; \
__gl_error_code = glGetError(); \
LIR_ASSERT(__gl_error_code == GL_NO_ERROR, "Error"); \
} while(0)
#endif


/** @def LIR_INCREMENT_GL_DRAWS_BY_ONE
 Increments the GL Draws counts by one.
 The number of calls per frame are displayed on the screen when the Director's stats are enabled.
 */
#define LIR_INCREMENT_GL_DRAWS(__n__) cocos2d::Director::getInstance()->getRenderer()->addDrawnBatches(__n__)
#define LIR_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(__drawcalls__, __vertices__) \
    do {                                                                \
        auto __renderer__ = cocos2d::Director::getInstance()->getRenderer();     \
        __renderer__->addDrawnBatches(__drawcalls__);                   \
        __renderer__->addDrawnVertices(__vertices__);                   \
    } while(0)

/*******************/
/** Notifications **/
/*******************/
/** @def AnimationFrameDisplayedNotification
 Notification name when a SpriteFrame is displayed
 */
#define AnimationFrameDisplayedNotification "CCAnimationFrameDisplayedNotification"

/*******************/
/** Notifications **/
/*******************/
/** @def Animate3DDisplayedNotification
 Notification name when a frame in Animate3D is played
 */
#define Animate3DDisplayedNotification "CCAnimate3DDisplayedNotification"

// new callbacks based on C++11
#define L_CALLBACK_0(__selector__,__target__, ...) std::bind(&__selector__,__target__, ##__VA_ARGS__)
#define L_CALLBACK_1(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, ##__VA_ARGS__)
#define L_CALLBACK_2(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, std::placeholders::_2, ##__VA_ARGS__)
#define L_CALLBACK_3(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, ##__VA_ARGS__)

#endif // __BASE_CCMACROS_H__
