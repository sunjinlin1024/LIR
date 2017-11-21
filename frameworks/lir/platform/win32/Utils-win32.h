#ifndef __UTILS_WIN32_H__
#define __UTILS_WIN32_H__

#include "platform/PlatformMacros.h"

#include <string>

NS_LIR_BEGIN

std::wstring LIR_DLL StringUtf8ToWideChar(const std::string& strUtf8);
std::string LIR_DLL StringWideCharToUtf8(const std::wstring& strWideChar);
std::string LIR_DLL UTF8StringToMultiByte(const std::string& strUtf8);

NS_LIR_END

#endif // __UTILS_WIN32_H__
