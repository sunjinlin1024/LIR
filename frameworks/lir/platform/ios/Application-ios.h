#ifndef __LIR_APPLICATION_IOS_H__
#define __LIR_APPLICATION_IOS_H__

#include "platform/PlatformConfig.h"
#if LIR_TARGET_PLATFORM == LIR_PLATFORM_IOS

#include "platform/Common.h"
#include "platform/ApplicationProtocol.h"

NS_LIR_BEGIN

class Rect;

class LIR_DLL Application : public ApplicationProtocol
{
public:
    /**
     * @js ctor
     */
    Application();
    /**
     * @js NA
     * @lua NA
     */
    virtual ~Application();
        
    /**
    @brief    Run the message loop.
    */
    int run();
        
    /**
    @brief    Get the current application instance.
    @return Current application instance pointer.
    */
    static Application* getInstance();
    
    /**
     @brief    Callback by Director for limit FPS.
     @param interval    The time, expressed in seconds, between current frame and next.
     */
    virtual void setAnimationInterval(float interval) override;

    /**
    @brief Get current language config
    @return Current language config
    */
    virtual LanguageType getCurrentLanguage() override;
    
    /**
     @brief Get current language iso 639-1 code
     @return Current language iso 639-1 code
     */
    virtual const char * getCurrentLanguageCode() override;
    
    /**
     @brief Get target platform
     */
    virtual Platform getTargetPlatform() override;
    
    /**
     @brief Get application version.
     */
    virtual std::string getVersion() override;
    
    /**
     @brief Open url in default browser
     @param String with url to open.
     @return true if the resource located by the URL was successfully opened; otherwise false.
     */
    virtual bool openURL(const std::string &url) override;

    /**
    @brief  This function will be called when the application screen size is changed.
    @param new width
    @param new height
    */
    virtual void applicationScreenSizeChanged(int newWidth, int newHeight);

protected:
    static Application * sm_pSharedApplication;
};

NS_LIR_END

#endif // LIR_PLATFORM_IOS

#endif    // end of __LIR_APPLICATION_IOS_H__
