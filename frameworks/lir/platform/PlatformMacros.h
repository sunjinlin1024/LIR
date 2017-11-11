#ifndef __LIR_PLATFORM_MACROS_H__
#define __LIR_PLATFORM_MACROS_H__

/**
 * Define some platform specific macros.
 */
#include "core/Config.h"
#include "platform/PlatformConfig.h"
#include "platform/PlatformDefine.h"

/** @def CREATE_FUNC(__TYPE__)
 * Define a create function for a specific type, such as Layer.
 *
 * @param __TYPE__  class type to add create(), such as Layer.
 */
#define CREATE_FUNC(__TYPE__) \
static __TYPE__* create() \
{ \
    __TYPE__ *pRet = new(std::nothrow) __TYPE__(); \
    if (pRet && pRet->init()) \
    { \
        pRet->autorelease(); \
        return pRet; \
    } \
    else \
    { \
        delete pRet; \
        pRet = nullptr; \
        return nullptr; \
    } \
}

/** @def NODE_FUNC(__TYPE__)
 * Define a node function for a specific type, such as Layer.
 *
 * @param __TYPE__  class type to add node(), such as Layer.
 * @deprecated  This interface will be deprecated sooner or later.
 */
#define NODE_FUNC(__TYPE__) \
LIR_DEPRECATED_ATTRIBUTE static __TYPE__* node() \
{ \
    __TYPE__ *pRet = new(std::nothrow) __TYPE__(); \
    if (pRet && pRet->init()) \
    { \
        pRet->autorelease(); \
        return pRet; \
    } \
    else \
    { \
        delete pRet; \
        pRet = NULL; \
        return NULL; \
    } \
}

/** @def LIR_ENABLE_CACHE_TEXTURE_DATA
 * Enable it if you want to cache the texture data.
 * Not enabling for Emscripten any more -- doesn't seem necessary and don't want
 * to be different from other platforms unless there's a good reason.
 * 
 * @since v0.99.5
 */
#if (LIR_TARGET_PLATFORM == LIR_PLATFORM_ANDROID) || (LIR_TARGET_PLATFORM == LIR_PLATFORM_WINRT) 
    #define LIR_ENABLE_CACHE_TEXTURE_DATA       1
#else
    #define LIR_ENABLE_CACHE_TEXTURE_DATA       0
#endif

#if (LIR_TARGET_PLATFORM == LIR_PLATFORM_ANDROID) || (LIR_TARGET_PLATFORM == LIR_PLATFORM_WIN32) || (LIR_TARGET_PLATFORM == LIR_PLATFORM_EMSCRIPTEN)
    /** Application will crash in glDrawElements function on some win32 computers and some android devices.
     *  Indices should be bound again while drawing to avoid this bug.
     */
    #define LIR_REBIND_INDICES_BUFFER  1
#else
    #define LIR_REBIND_INDICES_BUFFER  0
#endif

// Generic macros

/// @name namespace cocos2d
/// @{
#ifdef __cplusplus
    #define NS_LIR_BEGIN                     namespace lir {
    #define NS_LIR_END                       }
	#define USING_NS_LIR                     using namespace lir;
    #define NS_LIR                           ::lir
#else
    #define NS_LIR_BEGIN 
    #define NS_LIR_END 
    #define USING_NS_LIR 
    #define NS_LIR
#endif 
//  end of namespace group
/// @}

/** @def LIR_PROPERTY_READONLY 
 * It is used to declare a protected variable. We can use getter to read the variable.
 * 
 * @param varType     the type of variable.
 * @param varName     variable name.
 * @param funName     "get + funName" will be the name of the getter.
 * @warning   The getter is a public virtual function, you should rewrite it first.
 *            The variables and methods declared after LIR_PROPERTY_READONLY are all public.
 *            If you need protected or private, please declare.
 */
#define LIR_PROPERTY_READONLY(varType, varName, funName)\
protected: varType varName;\
public: virtual varType get##funName(void) const;

#define LIR_PROPERTY_READONLY_PASS_BY_REF(varType, varName, funName)\
protected: varType varName;\
public: virtual const varType& get##funName(void) const;

/** @def LIR_PROPERTY 
 * It is used to declare a protected variable.
 * We can use getter to read the variable, and use the setter to change the variable.
 *
 * @param varType     The type of variable.
 * @param varName     Variable name.
 * @param funName     "get + funName" will be the name of the getter.
 *                    "set + funName" will be the name of the setter.
 * @warning   The getter and setter are public virtual functions, you should rewrite them first.
 *            The variables and methods declared after LIR_PROPERTY are all public.
 *            If you need protected or private, please declare.
 */
#define LIR_PROPERTY(varType, varName, funName)\
protected: varType varName;\
public: virtual varType get##funName(void) const;\
public: virtual void set##funName(varType var);

#define LIR_PROPERTY_PASS_BY_REF(varType, varName, funName)\
protected: varType varName;\
public: virtual const varType& get##funName(void) const;\
public: virtual void set##funName(const varType& var);

/** @def LIR_SYNTHESIZE_READONLY 
 * It is used to declare a protected variable. We can use getter to read the variable.
 *
 * @param varType     The type of variable.
 * @param varName     Variable name.
 * @param funName     "get + funName" will be the name of the getter.
 * @warning   The getter is a public inline function.
 *            The variables and methods declared after LIR_SYNTHESIZE_READONLY are all public.
 *            If you need protected or private, please declare.
 */
#define LIR_SYNTHESIZE_READONLY(varType, varName, funName)\
protected: varType varName;\
public: virtual varType get##funName(void) const { return varName; }

#define LIR_SYNTHESIZE_READONLY_PASS_BY_REF(varType, varName, funName)\
protected: varType varName;\
public: virtual const varType& get##funName(void) const { return varName; }

/** @def LIR_SYNTHESIZE 
 * It is used to declare a protected variable.
 * We can use getter to read the variable, and use the setter to change the variable.
 *
 * @param varType     The type of variable.
 * @param varName     Variable name.
 * @param funName     "get + funName" will be the name of the getter.
 *                    "set + funName" will be the name of the setter.
 * @warning   The getter and setter are public inline functions.
 *            The variables and methods declared after LIR_SYNTHESIZE are all public.
 *            If you need protected or private, please declare.
 */
#define LIR_SYNTHESIZE(varType, varName, funName)\
protected: varType varName;\
public: virtual varType get##funName(void) const { return varName; }\
public: virtual void set##funName(varType var){ varName = var; }

#define LIR_SYNTHESIZE_PASS_BY_REF(varType, varName, funName)\
protected: varType varName;\
public: virtual const varType& get##funName(void) const { return varName; }\
public: virtual void set##funName(const varType& var){ varName = var; }

#define LIR_SYNTHESIZE_RETAIN(varType, varName, funName)    \
private: varType varName; \
public: virtual varType get##funName(void) const { return varName; } \
public: virtual void set##funName(varType var)   \
{ \
    if (varName != var) \
    { \
        LIR_SAFE_RETAIN(var); \
        LIR_SAFE_RELEASE(varName); \
        varName = var; \
    } \
} 

#define LIR_SAFE_DELETE(p)           do { delete (p); (p) = nullptr; } while(0)
#define LIR_SAFE_DELETE_ARRAY(p)     do { if(p) { delete[] (p); (p) = nullptr; } } while(0)
#define LIR_SAFE_FREE(p)             do { if(p) { free(p); (p) = nullptr; } } while(0)
#define LIR_SAFE_RELEASE(p)          do { if(p) { (p)->release(); } } while(0)
#define LIR_SAFE_RELEASE_NULL(p)     do { if(p) { (p)->release(); (p) = nullptr; } } while(0)
#define LIR_SAFE_RETAIN(p)           do { if(p) { (p)->retain(); } } while(0)
#define LIR_BREAK_IF(cond)           if(cond) break

#define __LIRLOGWITHFUNCTION(s, ...) \
    lir::log("%s : %s",__FUNCTION__, cocos2d::StringUtils::format(s, ##__VA_ARGS__).c_str())

/// @name Cocos2d debug
/// @{
#if !defined(COCOS2D_DEBUG) || COCOS2D_DEBUG == 0
#define CCLOG(...)       do {} while (0)
#define CCLOGINFO(...)   do {} while (0)
#define CCLOGERROR(...)  do {} while (0)
#define CCLOGWARN(...)   do {} while (0)

#elif COCOS2D_DEBUG == 1
#define CCLOG(format, ...)      cocos2d::log(format, ##__VA_ARGS__)
#define CCLOGERROR(format,...)  cocos2d::log(format, ##__VA_ARGS__)
#define CCLOGINFO(format,...)   do {} while (0)
#define CCLOGWARN(...) __LIRLOGWITHFUNCTION(__VA_ARGS__)

#elif COCOS2D_DEBUG > 1
#define CCLOG(format, ...)      cocos2d::log(format, ##__VA_ARGS__)
#define CCLOGERROR(format,...)  cocos2d::log(format, ##__VA_ARGS__)
#define CCLOGINFO(format,...)   cocos2d::log(format, ##__VA_ARGS__)
#define CCLOGWARN(...) __LIRLOGWITHFUNCTION(__VA_ARGS__)
#endif // COCOS2D_DEBUG

/** Lua engine debug */
#if !defined(COCOS2D_DEBUG) || COCOS2D_DEBUG == 0 || LIR_LUA_ENGINE_DEBUG == 0
#define LUALOG(...)
#else
#define LUALOG(format, ...)     cocos2d::log(format, ##__VA_ARGS__)
#endif // Lua engine debug

//  end of debug group
/// @}

/** @def LIR_DISALLOW_COPY_AND_ASSIGN(TypeName)
 * A macro to disallow the copy constructor and operator= functions.
 * This should be used in the private: declarations for a class
 */
#if defined(__GNUC__) && ((__GNUC__ >= 5) || ((__GNUG__ == 4) && (__GNUC_MINOR__ >= 4))) \
    || (defined(__clang__) && (__clang_major__ >= 3)) || (_MSC_VER >= 1800)
#define LIR_DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName &) = delete; \
    TypeName &operator =(const TypeName &) = delete;
#else
#define LIR_DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName &); \
    TypeName &operator =(const TypeName &);
#endif

/** @def LIR_DISALLOW_IMPLICIT_CONSTRUCTORS(TypeName)
 * A macro to disallow all the implicit constructors, namely the
 * default constructor, copy constructor and operator= functions.
 *
 * This should be used in the private: declarations for a class
 * that wants to prevent anyone from instantiating it. This is
 * especially useful for classes containing only static methods. 
 */
#define LIR_DISALLOW_IMPLICIT_CONSTRUCTORS(TypeName)    \
    TypeName();                                        \
    LIR_DISALLOW_COPY_AND_ASSIGN(TypeName)

/** @def LIR_DEPRECATED_ATTRIBUTE
 * Only certain compilers support __attribute__((deprecated)).
 */
#if defined(__GNUC__) && ((__GNUC__ >= 4) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 1)))
    #define LIR_DEPRECATED_ATTRIBUTE __attribute__((deprecated))
#elif _MSC_VER >= 1400 //vs 2005 or higher
    #define LIR_DEPRECATED_ATTRIBUTE __declspec(deprecated) 
#else
    #define LIR_DEPRECATED_ATTRIBUTE
#endif 

/** @def LIR_DEPRECATED(...)
 * Macro to mark things deprecated as of a particular version
 * can be used with arbitrary parameters which are thrown away.
 * e.g. LIR_DEPRECATED(4.0) or LIR_DEPRECATED(4.0, "not going to need this anymore") etc.
 */
#define LIR_DEPRECATED(...) LIR_DEPRECATED_ATTRIBUTE

/** @def LIR_FORMAT_PRINTF(formatPos, argPos)
 * Only certain compiler support __attribute__((format))
 *
 * @param formatPos 1-based position of format string argument.
 * @param argPos    1-based position of first format-dependent argument.
 */
#if defined(__GNUC__) && (__GNUC__ >= 4)
#define LIR_FORMAT_PRINTF(formatPos, argPos) __attribute__((__format__(printf, formatPos, argPos)))
#elif defined(__has_attribute)
  #if __has_attribute(format)
  #define LIR_FORMAT_PRINTF(formatPos, argPos) __attribute__((__format__(printf, formatPos, argPos)))
  #endif // __has_attribute(format)
#else
#define LIR_FORMAT_PRINTF(formatPos, argPos)
#endif

#if defined(_MSC_VER)
#define LIR_FORMAT_PRINTF_SIZE_T "%08lX"
#else
#define LIR_FORMAT_PRINTF_SIZE_T "%08zX"
#endif

#ifdef __GNUC__
#define LIR_UNUSED __attribute__ ((unused))
#else
#define LIR_UNUSED
#endif

/** @def LIR_REQUIRES_NULL_TERMINATION
 * 
 */
#if !defined(LIR_REQUIRES_NULL_TERMINATION)
    #if defined(__APPLE_LIR__) && (__APPLE_LIR__ >= 5549)
        #define LIR_REQUIRES_NULL_TERMINATION __attribute__((sentinel(0,1)))
    #elif defined(__GNUC__)
        #define LIR_REQUIRES_NULL_TERMINATION __attribute__((sentinel))
    #else
        #define LIR_REQUIRES_NULL_TERMINATION
    #endif
#endif

#endif // __LIR_PLATFORM_MACROS_H__
