
#include "platform/PlatformConfig.h"
#if LIR_TARGET_PLATFORM == LIR_PLATFORM_WIN32

#include "platform/Common.h"
#include "platform/StdC.h"
#include "platform/win32/Utils-win32.h"

NS_LIR_BEGIN

#define MAX_LEN       512  //(lir::kMaxLogLen + 1)

void MessageBox(const char * pszMsg, const char * pszTitle)
{
    std::wstring wsMsg = lir::StringUtf8ToWideChar(pszMsg);
	std::wstring wsTitle = lir::StringUtf8ToWideChar(pszTitle);
    MessageBoxW(nullptr, wsMsg.c_str(), wsTitle.c_str(), MB_OK);
}

void LuaLog(const char *pszMsg)
{
	OutputDebugStringW(lir::StringUtf8ToWideChar(pszMsg).c_str());
    OutputDebugStringA("\n");

    puts(UTF8StringToMultiByte(pszMsg).c_str());
}

NS_LIR_END

#endif // LIR_TARGET_PLATFORM == CC_PLATFORM_WIN32
