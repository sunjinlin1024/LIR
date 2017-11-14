#ifndef ___LIRFILEUTILS_H__
#define ___LIRFILEUTILS_H__

#include <string>
#include <vector>
#include <unordered_map>
#include <type_traits>

#include "platform/PlatformMacros.h"
#include "io/FileHandler.h"
//#include "base/ccTypes.h"
//#include "base/CCValue.h"
//#include "base/CCData.h"
//#include "base/CCAsyncTaskPool.h"
//#include "base/CCScheduler.h"
//#include "base/CCDirector.h"

NS_LIR_BEGIN



/** Helper class to handle file operations. */
class LIR_DLL FileUtils
{
public:

    /**
     *  Gets the instance of FileUtils.
     */
    static FileUtils* getInstance();

    static void destroyInstance();

    static void setDelegate(FileUtils *delegate);

		
	FileUtils();

   


    virtual void purgeCachedEntries();

	FileStatus getContents(const std::string& filename, Buffer* buffer);

	std::string fullPathForFilename(const std::string& filename) const;

	virtual bool isAbsolutePath(const std::string& path) const;

	std::string getSuitableFOpen(const std::string& filenameUtf8) const;

	std::string getNewFilename(const std::string &filename) const;

	virtual bool isFileExistInternal(const std::string& filename) const{
		return true;
	};// = 0;

	bool isPopupNotify() const;

	void setPopupNotify(bool notify);


	virtual bool init();
	virtual ~FileUtils();
protected:

    

    virtual std::string getPathForFilename(const std::string& filename, const std::string& resolutionDirectory, const std::string& searchPath) const;

    virtual std::string getFullPathForDirectoryAndFilename(const std::string& directory, const std::string& filename) const;


    std::vector<std::string> _searchResolutionsOrderArray;

    std::vector<std::string> _searchPathArray;

    std::vector<std::string> _originalSearchPaths;

    std::string _defaultResRootPath;

    mutable std::unordered_map<std::string, std::string> _fullPathCache;
	
    std::string _writablePath;

    static FileUtils* s_sharedFileUtils;

//    template<typename T, typename R, typename ...ARGS>
//    static void performOperationOffthread(T&& action, R&& callback, ARGS&& ...args)
//    {
//
//        // Visual Studio 2013 does not support using std::bind to forward template parameters into
//        // a lambda. To get around this, we will just copy these arguments via lambda capture
//#if defined(_MSC_VER) && _MSC_VER  < 1900 
//        auto lambda = [action, callback, args...]() 
//        {
//            Director::getInstance()->getScheduler()->performFunctionInCocosThread(std::bind(callback, action(args...)));
//        };
//#else
//        // As cocos2d-x uses c++11, we will use std::bind to leverage move sematics to
//        // move our arguments into our lambda, to potentially avoid copying. 
//        auto lambda = std::bind([](const T& actionIn, const R& callbackIn, const ARGS& ...argsIn)
//        {
//            Director::getInstance()->getScheduler()->performFunctionInCocosThread(std::bind(callbackIn, actionIn(argsIn...)));
//        }, std::forward<T>(action), std::forward<R>(callback), std::forward<ARGS>(args)...);
//#endif
//
//        AsyncTaskPool::getInstance()->enqueue(AsyncTaskPool::TaskType::TASK_IO, [](void*){}, nullptr, std::move(lambda));
//    }
};

// end of support group
/** @} */

NS_LIR_END

#endif    // ___LIRFILEUTILS_H__
