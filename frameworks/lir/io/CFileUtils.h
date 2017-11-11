#ifndef ___LIRFILEUTILS_H__
#define ___LIRFILEUTILS_H__

#include <string>
#include <vector>
#include <unordered_map>
#include <type_traits>

#include "platform/LPlatformMacros.h"
//#include "base/ccTypes.h"
//#include "base/CCValue.h"
//#include "base/CCData.h"
//#include "base/CCAsyncTaskPool.h"
//#include "base/CCScheduler.h"
//#include "base/CCDirector.h"

NS_LIR_BEGIN


/** Helper class to handle file operations. */
class CC_DLL FileUtils
{
public:
    /**
     *  Gets the instance of FileUtils.
     */
    static FileUtils* getInstance();
    static void destroyInstance();


    static void setDelegate(FileUtils *delegate);

    virtual ~FileUtils();


    virtual void purgeCachedEntries();

    virtual std::string getStringFromFile(const std::string& filename);
    

    virtual void getStringFromFile(const std::string& path, std::function<void(std::string)> callback);

    virtual Data getDataFromFile(const std::string& filename);
    

    virtual void getDataFromFile(const std::string& filename, std::function<void(Data)> callback);

    enum class Status
    {
        OK = 0,
        NotExists = 1, // File not exists
        OpenFailed = 2, // Open file failed.
        ReadFailed = 3, // Read failed
        NotInitialized = 4, // FileUtils is not initializes
        TooLarge = 5, // The file is too large (great than 2^32-1)
        ObtainSizeFailed = 6 // Failed to obtain the file size.
    };

    virtual Status getContents(const std::string& filename, ResizableBuffer* buffer);

    virtual unsigned char* getFileData(const std::string& filename, const char* mode, ssize_t *size);

    virtual unsigned char* getFileDataFromZip(const std::string& zipFilePath, const std::string& filename, ssize_t *size);

    virtual std::string fullPathForFilename(const std::string &filename) const;

    virtual void loadFilenameLookupDictionaryFromFile(const std::string &filename);

    virtual void setFilenameLookupDictionary(const ValueMap& filenameLookupDict);

    virtual std::string fullPathFromRelativeFile(const std::string &filename, const std::string &relativeFile);

    virtual void setSearchResolutionsOrder(const std::vector<std::string>& searchResolutionsOrder);

    virtual void addSearchResolutionsOrder(const std::string &order,const bool front=false);

    virtual const std::vector<std::string>& getSearchResolutionsOrder() const;

    virtual void setSearchPaths(const std::vector<std::string>& searchPaths);

   
    const std::string& getDefaultResourceRootPath() const;

    void setDefaultResourceRootPath(const std::string& path);

    void addSearchPath(const std::string & path, const bool front=false);

    virtual const std::vector<std::string>& getSearchPaths() const;

    virtual const std::vector<std::string>& getOriginalSearchPaths() const;

    virtual std::string getWritablePath() const = 0;

    virtual void setWritablePath(const std::string& writablePath);

    virtual void setPopupNotify(bool notify);

    virtual bool isPopupNotify() const;

    virtual bool writeToFile(const ValueMap& dict, const std::string& fullPath);

    virtual bool writeStringToFile(const std::string& dataStr, const std::string& fullPath);

    virtual void writeStringToFile(std::string dataStr, const std::string& fullPath, std::function<void(bool)> callback);
    
    virtual bool writeDataToFile(const Data& data, const std::string& fullPath);
    
    virtual void writeDataToFile(Data data, const std::string& fullPath, std::function<void(bool)> callback);

    virtual bool writeValueMapToFile(const ValueMap& dict, const std::string& fullPath);

    virtual void writeValueMapToFile(ValueMap dict, const std::string& fullPath, std::function<void(bool)> callback);

    virtual bool writeValueVectorToFile(const ValueVector& vecData, const std::string& fullPath);

    virtual void writeValueVectorToFile(ValueVector vecData, const std::string& fullPath, std::function<void(bool)> callback);

    virtual std::string getSuitableFOpen(const std::string& filenameUtf8) const;

    virtual ValueVector getValueVectorFromFile(const std::string& filename);

    virtual bool isFileExist(const std::string& filename) const;

    virtual void isFileExist(const std::string& filename, std::function<void(bool)> callback);

    virtual std::string getFileExtension(const std::string& filePath) const;

    virtual bool isAbsolutePath(const std::string& path) const;

    virtual bool isDirectoryExist(const std::string& dirPath) const;

    virtual void isDirectoryExist(const std::string& fullPath, std::function<void(bool)> callback);

    virtual bool createDirectory(const std::string& dirPath);

    virtual void createDirectory(const std::string& dirPath, std::function<void(bool)> callback);

    virtual bool removeDirectory(const std::string& dirPath);

    virtual void removeDirectory(const std::string& dirPath, std::function<void(bool)> callback);

    virtual bool removeFile(const std::string &filepath);

    virtual void removeFile(const std::string &filepath, std::function<void(bool)> callback);

    virtual bool renameFile(const std::string &path, const std::string &oldname, const std::string &name);

    virtual void renameFile(const std::string &path, const std::string &oldname, const std::string &name, std::function<void(bool)> callback);

    virtual bool renameFile(const std::string &oldfullpath, const std::string &newfullpath);

    virtual void renameFile(const std::string &oldfullpath, const std::string &newfullpath, std::function<void(bool)> callback);

    virtual long getFileSize(const std::string &filepath);

    virtual void getFileSize(const std::string &filepath, std::function<void(long)> callback);

    virtual std::vector<std::string> listFiles(const std::string& dirPath) const;

    virtual void listFilesRecursively(const std::string& dirPath, std::vector<std::string> *files) const;

    const std::unordered_map<std::string, std::string>& getFullPathCache() const { return _fullPathCache; }

    virtual std::string getNewFilename(const std::string &filename) const;

protected:

    FileUtils();

    virtual bool init();

    virtual bool isFileExistInternal(const std::string& filename) const = 0;

    virtual bool isDirectoryExistInternal(const std::string& dirPath) const;

    virtual std::string getPathForFilename(const std::string& filename, const std::string& resolutionDirectory, const std::string& searchPath) const;

    virtual std::string getFullPathForDirectoryAndFilename(const std::string& directory, const std::string& filename) const;

    ValueMap _filenameLookupDict;

    std::vector<std::string> _searchResolutionsOrderArray;

    std::vector<std::string> _searchPathArray;

    std::vector<std::string> _originalSearchPaths;

    std::string _defaultResRootPath;

    mutable std::unordered_map<std::string, std::string> _fullPathCache;
	
    std::string _writablePath;

    static FileUtils* s_sharedFileUtils;

    virtual void valueMapCompact(ValueMap& valueMap);
    virtual void valueVectorCompact(ValueVector& valueVector);

    template<typename T, typename R, typename ...ARGS>
    static void performOperationOffthread(T&& action, R&& callback, ARGS&& ...args)
    {

        // Visual Studio 2013 does not support using std::bind to forward template parameters into
        // a lambda. To get around this, we will just copy these arguments via lambda capture
#if defined(_MSC_VER) && _MSC_VER  < 1900 
        auto lambda = [action, callback, args...]() 
        {
            Director::getInstance()->getScheduler()->performFunctionInCocosThread(std::bind(callback, action(args...)));
        };
#else
        // As cocos2d-x uses c++11, we will use std::bind to leverage move sematics to
        // move our arguments into our lambda, to potentially avoid copying. 
        auto lambda = std::bind([](const T& actionIn, const R& callbackIn, const ARGS& ...argsIn)
        {
            Director::getInstance()->getScheduler()->performFunctionInCocosThread(std::bind(callbackIn, actionIn(argsIn...)));
        }, std::forward<T>(action), std::forward<R>(callback), std::forward<ARGS>(args)...);
#endif

        AsyncTaskPool::getInstance()->enqueue(AsyncTaskPool::TaskType::TASK_IO, [](void*){}, nullptr, std::move(lambda));
    }
};

// end of support group
/** @} */

NS__LIREND

#endif    // ___LIRFILEUTILS_H__
