#ifndef __LIR_FILEUTILS_APPLE_H__
#define __LIR_FILEUTILS_APPLE_H__

#include <memory>
#include <string>
#include <vector>

#include "io/FileUtils.h"
#include "platform/PlatformMacros.h"
//#include "base/ccTypes.h"

NS_LIR_BEGIN

/**
 * @addtogroup platform
 * @{
 */

//! @brief  Helper class to handle file operations
class LIR_DLL FileUtilsApple : public FileUtils
{
public:
    FileUtilsApple();
    virtual ~FileUtilsApple();
    /* override functions */
    virtual std::string getWritablePath() const override;
    virtual std::string getFullPathForDirectoryAndFilename(const std::string& directory, const std::string& filename) const override;

//    virtual ValueMap getValueMapFromFile(const std::string& filename) override;
//    virtual ValueMap getValueMapFromData(const char* filedata, int filesize)override;
//    virtual bool writeToFile(const ValueMap& dict, const std::string& fullPath) override;
//
//    virtual ValueVector getValueVectorFromFile(const std::string& filename) override;
#if LIR_FILEUTILS_APPLE_ENABLE_OBJC
    void setBundle(NSBundle* bundle);
#endif
    
    virtual bool createDirectory(const std::string& path) override;
private:
    virtual bool isFileExistInternal(const std::string& filePath) const override;
    virtual bool removeDirectory(const std::string& dirPath) override;
//    virtual void valueMapCompact(ValueMap& valueMap) override;
//    virtual void valueVectorCompact(ValueVector& valueVector) override;

    struct IMPL;
    std::unique_ptr<IMPL> pimpl_;
};

// end of platform group
/// @}

NS_LIR_END

#endif    // __LIR_FILEUTILS_APPLE_H__
