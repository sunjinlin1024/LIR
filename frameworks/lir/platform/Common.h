#ifndef __LIR_COMMON_H__
#define __LIR_COMMON_H__
/// @cond DO_NOT_SHOW

#include "platform/PlatformMacros.h"

NS_LIR_BEGIN

/**
 * @addtogroup platform
 * @{
 */

/**
 * lua can not deal with ...
 */
void LIR_DLL LuaLog(const char * format);

/**
@brief Pop out a message box
*/
void LIR_DLL MessageBox(const char * msg, const char * title);

/**
@brief Enum the language type supported now
*/
enum class LanguageType
{
    ENGLISH = 0,
    CHINESE,
    FRENCH,
    ITALIAN,
    GERMAN,
    SPANISH,
    DUTCH,
    RUSSIAN,
    KOREAN,
    JAPANESE,
    HUNGARIAN,
    PORTUGUESE,
    ARABIC,
    NORWEGIAN,
    POLISH,
    TURKISH,
    UKRAINIAN,
    ROMANIAN,
    BULGARIAN
};

// END of platform group
/// @}

NS_LIR_END

/// @endcond
#endif    // __LIR_COMMON_H__
