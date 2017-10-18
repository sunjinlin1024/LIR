/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2017 Chukong Technologies Inc.

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#ifndef __LCONFIG_H__
#define __LCONFIG_H__

#include "platform/LPlatformConfig.h"

/**
 * @file
 * cocos2d (cc) configuration file.
*/

/** @def LIR_ENABLE_STACKABLE_ACTIONS
 * If enabled, actions that alter the position property (eg: MoveBy, JumpBy, BezierBy, etc..) will be stacked.
 * If you run 2 or more 'position' actions at the same time on a node, then end position will be the sum of all the positions.
 * If disabled, only the last run action will take effect.
 * Enabled by default. Disable to be compatible with v2.0 and older versions.
 * @since v2.1
 */
#ifndef LIR_ENABLE_STACKABLE_ACTIONS
#define LIR_ENABLE_STACKABLE_ACTIONS 1
#endif

/** @def LIR_ENABLE_GL_STATE_CACHE
 * If enabled, cocos2d will maintain an OpenGL state cache internally to avoid unnecessary switches.
 * In order to use them, you have to use the following functions, instead of the GL ones:
 *  - ccGLUseProgram() instead of glUseProgram().
 *  - GL::deleteProgram() instead of glDeleteProgram().
 *  - GL::blendFunc() instead of glBlendFunc().

 * If this functionality is disabled, then ccGLUseProgram(), GL::deleteProgram(), GL::blendFunc() will call the GL ones, without using the cache.

 * It is recommended to enable whenever possible to improve speed.
 * If you are migrating your code from GL ES 1.1, then keep it disabled. Once all your code works as expected, turn it on.

 * Default value: Enabled by default

 * @since v2.0.0
 */
#ifndef LIR_ENABLE_GL_STATE_CACHE
#define LIR_ENABLE_GL_STATE_CACHE 1
#endif

/** @def LIR_FIX_ARTIFACTS_BY_STRECHING_TEXEL
 * If enabled, the texture coordinates will be calculated by using this formula:
 * - texCoord.left = (rect.origin.x*2+1) / (texture.wide*2);
 * - texCoord.right = texCoord.left + (rect.size.width*2-2)/(texture.wide*2);

 * The same for bottom and top.

 * This formula prevents artifacts by using 99% of the texture.
 * The "correct" way to prevent artifacts is by using the spritesheet-artifact-fixer.py or a similar tool.

 * Affected nodes:
 * - Sprite / SpriteBatchNode and subclasses: LabelBMFont, TMXTiledMap.
 * - LabelAtlas.
 * - QuadParticleSystem.
 * - TileMap.

 * To enabled set it to 1. Disabled by default.

 * @since v0.99.5
 */
#ifndef LIR_FIX_ARTIFACTS_BY_STRECHING_TEXEL
#define LIR_FIX_ARTIFACTS_BY_STRECHING_TEXEL 0
#endif

/** @def LIR_DIRECTOR_STATS_INTERVAL
 * Seconds between FPS updates.
 * 0.5 seconds, means that the FPS number will be updated every 0.5 seconds.
 * Having a bigger number means a more reliable FPS.

 * Default value: 0.5f
 */
#ifndef LIR_DIRECTOR_STATS_INTERVAL
#define LIR_DIRECTOR_STATS_INTERVAL (0.5f)
#endif

/** @def LIR_DIRECTOR_FPS_POSITION
 * Position of the FPS.

 * Default: 0,0 (bottom-left corner).
 */
#ifndef LIR_DIRECTOR_FPS_POSITION
#define LIR_DIRECTOR_FPS_POSITION Vec2(0,0)
#endif

/** @def LIR_DIRECTOR_DISPATCH_FAST_EVENTS
 * If enabled, and only when it is used with FastDirector, the main loop will wait 0.04 seconds to
 * dispatch all the events, even if there are not events to dispatch.
 * If your game uses lot's of events (eg: touches) it might be a good idea to enable this feature.
 * Otherwise, it is safe to leave it disabled.

 * To enable set it to 1. Disabled by default.

 * @warning This feature is experimental.
 */
#ifndef LIR_DIRECTOR_DISPATCH_FAST_EVENTS
 #define LIR_DIRECTOR_DISPATCH_FAST_EVENTS 0
#endif

/** @def LIR_DIRECTOR_MAC_USE_DISPLAY_LINK_THREAD
 * If enabled, cocos2d-mac will run on the Display Link thread. If disabled cocos2d-mac will run in its own thread.
 * If enabled, the images will be drawn at the "correct" time, but the events might not be very responsive.
 * If disabled, some frames might be skipped, but the events will be dispatched as they arrived.
 * To enable set it to a 1, to disable it set to 0. Enabled by default.
 * Only valid for cocos2d-mac. Not supported on cocos2d-ios.
 */
#ifndef LIR_DIRECTOR_MAC_USE_DISPLAY_LINK_THREAD
#define LIR_DIRECTOR_MAC_USE_DISPLAY_LINK_THREAD 1
#endif

/** @def LIR_NODE_RENDER_SUBPIXEL
 * If enabled, the Node objects (Sprite, Label,etc) will be able to render in subpixels.
 * If disabled, integer pixels will be used.
 * To enable set it to 1. Enabled by default.
 */
#ifndef LIR_NODE_RENDER_SUBPIXEL
#define LIR_NODE_RENDER_SUBPIXEL 1
#endif

/** @def LIR_SPRITEBATCHNODE_RENDER_SUBPIXEL
 * If enabled, the Sprite objects rendered with SpriteBatchNode will be able to render in subpixels.
 * If disabled, integer pixels will be used.
 * To enable set it to 1. Enabled by default.
 */
#ifndef LIR_SPRITEBATCHNODE_RENDER_SUBPIXEL
#define LIR_SPRITEBATCHNODE_RENDER_SUBPIXEL    1
#endif

/** @def LIR_TEXTURE_ATLAS_USE_VAO
 * By default, TextureAtlas (used by many cocos2d classes) will use VAO (Vertex Array Objects).
 * Apple recommends its usage but they might consume a lot of memory, specially if you use many of them.
 * So for certain cases, where you might need hundreds of VAO objects, it might be a good idea to disable it.
 * To disable it set it to 0. Enabled by default.
 * If a device doesn't support VAO though it claims to support should add exceptions list here.
 */
#ifndef LIR_TEXTURE_ATLAS_USE_VAO
#define LIR_TEXTURE_ATLAS_USE_VAO 1
#endif


/** @def LIR_USE_LA88_LABELS
 * If enabled, it will use LA88 (Luminance Alpha 16-bit textures) for LabelTTF objects.
 * If it is disabled, it will use A8 (Alpha 8-bit textures).
 * LA88 textures are 6% faster than A8 textures, but they will consume 2x memory.
 * This feature is enabled by default.
 * @since v0.99.5
 */
#ifndef LIR_USE_LA88_LABELS
#define LIR_USE_LA88_LABELS 1
#endif

/** @def LIR_SPRITE_DEBUG_DRAW
 * If enabled, all subclasses of Sprite will draw a bounding box.
 * Useful for debugging purposes only. It is recommended to leave it disabled.
 * To enable set it to a value different than 0. Disabled by default:
 * 0 -- disabled
 * 1 -- draw bounding box
 * 2 -- draw texture box
 */
#ifndef LIR_SPRITE_DEBUG_DRAW
#define LIR_SPRITE_DEBUG_DRAW 0
#endif

/** @def LIR_LABEL_DEBUG_DRAW
* If enabled, all subclasses of Label will draw a bounding box.
* Useful for debugging purposes only. It is recommended to leave it disabled.
* To enable set it to a value different than 0. Disabled by default:
* 0 -- disabled
* 1 -- draw bounding box
*/
#ifndef LIR_LABEL_DEBUG_DRAW
#define LIR_LABEL_DEBUG_DRAW 0
#endif

/** @def LIR_SPRITEBATCHNODE_DEBUG_DRAW
 * If enabled, all subclasses of Sprite that are rendered using an SpriteBatchNode draw a bounding box.
 * Useful for debugging purposes only. It is recommended to leave it disabled.
 * To enable set it to a value different than 0. Disabled by default.
 */
#ifndef LIR_SPRITEBATCHNODE_DEBUG_DRAW
#define LIR_SPRITEBATCHNODE_DEBUG_DRAW 0
#endif

/** @def LIR_LABELBMFONT_DEBUG_DRAW
 * If enabled, all subclasses of LabelBMFont will draw a bounding box.
 * Useful for debugging purposes only. It is recommended to leave it disabled.
 * To enable set it to a value different than 0. Disabled by default.
 */
#ifndef LIR_LABELBMFONT_DEBUG_DRAW
#define LIR_LABELBMFONT_DEBUG_DRAW 0
#endif

/** @def LIR_LABELATLAS_DEBUG_DRAW
 * If enabled, all subclasses of LabeltAtlas will draw a bounding box
 * Useful for debugging purposes only. It is recommended to leave it disabled.
 * To enable set it to a value different than 0. Disabled by default.
 */
#ifndef LIR_LABELATLAS_DEBUG_DRAW
#define LIR_LABELATLAS_DEBUG_DRAW 0
#endif

/** @def LIR_NODE_DEBUG_VERIFY_EVENT_LISTENERS
 * If enabled (in conjunction with assertion macros) will verify on Node destruction that the node being destroyed has no event
 * listeners still associated with it in the event dispatcher. This can be used to track down problems where the event dispatch
 * system has dangling pointers to destroyed nodes.
 * Note: event listener verification will always be disabled in builds where assertions are disabled regardless of this setting.
 */
#ifndef LIR_NODE_DEBUG_VERIFY_EVENT_LISTENERS
#define LIR_NODE_DEBUG_VERIFY_EVENT_LISTENERS 0
#endif

/** @def LIR_ENABLE_PROFILERS
 * If enabled, will activate various profilers within cocos2d. This statistical data will be output to the console
 * once per second showing average time (in milliseconds) required to execute the specific routine(s).
 * Useful for debugging purposes only. It is recommended to leave it disabled.
 * To enable set it to a value different than 0. Disabled by default.
 */
#ifndef LIR_ENABLE_PROFILERS
#define LIR_ENABLE_PROFILERS 0
#endif

/** Enable Lua engine debug log. */
#ifndef LIR_LUA_ENGINE_DEBUG
#define LIR_LUA_ENGINE_DEBUG 0
#endif

/** Use physics integration API. */
#ifndef LIR_USE_PHYSICS
#define LIR_USE_PHYSICS 1
#endif

/** Use 3d physics integration API. */
#ifndef LIR_USE_3D_PHYSICS
#if (LIR_TARGET_PLATFORM == LIR_PLATFORM_IOS || LIR_TARGET_PLATFORM == LIR_PLATFORM_MAC || LIR_TARGET_PLATFORM == LIR_PLATFORM_WIN32 || LIR_TARGET_PLATFORM == LIR_PLATFORM_ANDROID || LIR_TARGET_PLATFORM == LIR_PLATFORM_LINUX || LIR_TARGET_PLATFORM == LIR_PLATFORM_WINRT)
#define LIR_USE_3D_PHYSICS 1
#endif
#endif

#if (LIR_USE_3D_PHYSICS)
/** Use bullet physics engine. */
#ifndef LIR_ENABLE_BULLET_INTEGRATION
#define LIR_ENABLE_BULLET_INTEGRATION 1
#endif
#endif

/** Use 3D navigation API */
#ifndef LIR_USE_NAVMESH
#define LIR_USE_NAVMESH 1
#endif

/** Use culling or not. */
#ifndef LIR_USE_CULLING
#define LIR_USE_CULLING 1
#endif

/** Support PNG or not. If your application don't use png format picture, you can undefine this macro to save package size.
*/
#ifndef LIR_USE_PNG
#define LIR_USE_PNG  1
#endif // LIR_USE_PNG

/** Support JPEG or not. If your application don't use jpeg format picture, you can undefine this macro to save package size.
 */
#ifndef LIR_USE_JPEG
#define LIR_USE_JPEG  1
#endif // LIR_USE_JPEG

/** Support TIFF or not. If your application don't use TIFF format picture, you can undefine this macro to save package size.
 */
#ifndef LIR_USE_TIFF
#define LIR_USE_TIFF  1
#endif // LIR_USE_TIFF

/** Support webp or not. If your application don't use webp format picture, you can undefine this macro to save package size.
 */
#ifndef LIR_USE_WEBP
#if (LIR_TARGET_PLATFORM != LIR_PLATFORM_WINRT)
#define LIR_USE_WEBP  1
#endif
#endif // LIR_USE_WEBP

 /** Support WIC (Windows Image Component) or not. Replaces PNG, TIFF and JPEG
 */
#ifndef LIR_USE_WIC
#if (LIR_TARGET_PLATFORM == LIR_PLATFORM_WINRT)
#define LIR_USE_WIC  1
#undef LIR_USE_TIFF
#undef LIR_USE_JPEG
#undef LIR_USE_PNG
#endif
#endif // LIR_USE_WIC

/** Enable Script binding. */
#ifndef LIR_ENABLE_SCRIPT_BINDING
#define LIR_ENABLE_SCRIPT_BINDING 1
#endif

/** When LIR_ENABLE_SCRIPT_BINDING and LIR_ENABLE_GC_FOR_NATIVE_OBJECTS are both 1
 then the Garbage collector will release the native objects, only when the JS/Lua objets
 are collected.
 The benefit is that users don't need to retain/release the JS/Lua objects manually.

 By default this behavior is disabled by default
 */
#ifdef LIR_ENABLE_SCRIPT_BINDING
  #ifndef LIR_ENABLE_GC_FOR_NATIVE_OBJECTS
  #define LIR_ENABLE_GC_FOR_NATIVE_OBJECTS 0
  #endif
#endif

/** @def LIR_CONSTRUCTOR_ACCESS
 * Indicate the init functions access modifier. If value equals to protected, then these functions are protected.
 * If value equals to public, these functions are public,
 * protected by default.
 */
#ifndef LIR_CONSTRUCTOR_ACCESS
  #ifdef LIR_ENABLE_SCRIPT_BINDING
    #define LIR_CONSTRUCTOR_ACCESS public
  #else
    #define LIR_CONSTRUCTOR_ACCESS protected
  #endif
#endif

/** @def LIR_ENABLE_ALLOCATOR
 * Turn on creation of global allocator and pool allocators
 * as specified by LIR_ALLOCATOR_GLOBAL below.
 */
#ifndef LIR_ENABLE_ALLOCATOR
# define LIR_ENABLE_ALLOCATOR 0
#endif

/** @def LIR_ENABLE_ALLOCATOR_DIAGNOSTICS
 * Turn on debugging of allocators. This is slower, uses
 * more memory, and should not be used for production builds.
 */
#ifndef LIR_ENABLE_ALLOCATOR_DIAGNOSTICS
# define LIR_ENABLE_ALLOCATOR_DIAGNOSTICS LIR_ENABLE_ALLOCATOR
#endif

/** @def LIR_ENABLE_ALLOCATOR_GLOBAL_NEW_DELETE
 * Turn on override of global new and delete
 * as specified by LIR_ALLOCATOR_GLOBAL_NEW_DELETE below.
 */
#ifndef LIR_ENABLE_ALLOCATOR_GLOBAL_NEW_DELETE
# define LIR_ENABLE_ALLOCATOR_GLOBAL_NEW_DELETE 0
# endif//LIR_ENABLE_ALLOCATOR_GLOBAL_NEW_DELETE

/** @def LIR_ALLOCATOR_GLOBAL
 * Specify allocator to use for global allocator.
 */
#ifndef LIR_ALLOCATOR_GLOBAL
# define LIR_ALLOCATOR_GLOBAL cocos2d::allocator::AllocatorStrategyDefault
#endif

/** @def LIR_ALLOCATOR_GLOBAL_NEW_DELETE
 * Specify allocator to use when overriding of new and delete.
 */
#ifndef LIR_ALLOCATOR_GLOBAL_NEW_DELETE
# define LIR_ALLOCATOR_GLOBAL_NEW_DELETE cocos2d::allocator::AllocatorStrategyGlobalSmallBlock
#endif

#ifndef LIR_FILEUTILS_APPLE_ENABLE_OBJC
#define LIR_FILEUTILS_APPLE_ENABLE_OBJC  1
#endif

/** @def LIR_ENABLE_PREMULTIPLIED_ALPHA
 * If enabled, all textures will be preprocessed to multiply its rgb components
 * by its alpha component.
 */
#ifndef LIR_ENABLE_PREMULTIPLIED_ALPHA
# define LIR_ENABLE_PREMULTIPLIED_ALPHA 1
#endif

#endif // __CCCONFIG_H__
