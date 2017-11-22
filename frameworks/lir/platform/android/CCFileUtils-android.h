#ifndef __LIR_FILEUTILS_ANDROID_H__
#define __LIR_FILEUTILS_ANDROID_H__

#include "platform/PlatformConfig.h"
#if LIR_TARGET_PLATFORM == LIR_PLATFORM_ANDROID

#include "platform/FileUtils.h"
#include "platform/PlatformMacros.h"
#include <string>
#include <vector>
#include "jni.h"
#include "android/asset_manager.h"

NS_LIR_BEGIN

class ZipFile;

/**
 * @addtogroup platform
 * @{
 */

//! @brief  Helper class to handle file operations
class LIR_DLL FileUtilsAndroid : public FileUtils
{
    friend class FileUtils;
public:
    FileUtilsAndroid();
    /**
     * @js NA
     * @lua NA
     */
    virtual ~FileUtilsAndroid();

    static void setassetmanager(AAssetManager* a);
    static AAssetManager* getAssetManager() { return assetmanager; }
    static ZipFile* getObbFile() { return obbfile; }

    /* override functions */
    bool init() override;

    virtual std::string getNewFilename(const std::string &filename) const override;

    virtual FileUtils::Status getContents(const std::string& filename, ResizableBuffer* buffer) override;

    virtual std::string getWritablePath() const override;
    virtual bool isAbsolutePath(const std::string& strPath) const override;

private:
    virtual bool isFileExistInternal(const std::string& strFilePath) const override;
    virtual bool isDirectoryExistInternal(const std::string& dirPath) const override;

    static AAssetManager* assetmanager;
    static ZipFile* obbfile;
};

// end of platform group
/// @}

NS_LIR_END

#endif // LIR_TARGET_PLATFORM == LIR_PLATFORM_ANDROID

#endif // __LIR_FILEUTILS_ANDROID_H__
