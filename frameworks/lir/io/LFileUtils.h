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


class LIR_DLL Buffer
{
public:
	Buffer() :_size(0),_buff(nullptr){};
	~Buffer();

	void resize(size_t size){
		if (size ==_size)
		{
			return;
		}
		else if (size >_size)
		{
			realloc(_buff, size);
		}
		_size = size;
	}

	unsigned char* buffer(){
		return _buff;
	}

private:
	size_t _size;
	unsigned char* _buff;
};

/** Helper class to handle file operations. */
class LIR_DLL FileUtils
{
public:
	enum Status
	{
		OK = 0,
		NotExists = 1, // File not exists
		OpenFailed = 2, // Open file failed.
		ReadFailed = 3, // Read failed
		NotInitialized = 4, // FileUtils is not initializes
		TooLarge = 5, // The file is too large (great than 2^32-1)
		ObtainSizeFailed = 6 // Failed to obtain the file size.
	};
    /**
     *  Gets the instance of FileUtils.
     */
    static FileUtils* getInstance();

    static void destroyInstance();

    static void setDelegate(FileUtils *delegate);

		
	FileUtils();

    virtual ~FileUtils();


    virtual void purgeCachedEntries();

	Status getContents(const std::string& filename,Buffer* buffer);

	std::string fullPathForFilename(const std::string &filename) const;

	virtual bool isAbsolutePath(const std::string& path) const;

	std::string getSuitableFOpen(const std::string& filenameUtf8) const;

	std::string getNewFilename(const std::string &filename) const;


	bool isPopupNotify() const;

	void setPopupNotify(bool notify);

protected:

    

    virtual bool init();

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
