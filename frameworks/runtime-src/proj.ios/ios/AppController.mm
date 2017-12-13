#import <UIKit/UIKit.h>
#import "lir.h"

#import "AppController.h"
#import "AppDelegate.h"
//#import "util/GameLuaOCBridge.h"
#import <SystemConfiguration/SystemConfiguration.h>
#import "ViewController.h"
//#import "platform/ios/CCEAGLView-ios.h"

//#import "manual/CCLuaEngine.h"
//#import "util/ProjectMgr/ProjectMgr.h"


@implementation AppController

#pragma mark -
#pragma mark Application lifecycle

// lir application instance
static AppDelegate s_sharedApplication;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    lir::Application *app = lir::Application::getInstance();
    app->initGLContextAttrs();
//    cocos2d::GLViewImpl::convertAttrs();

    // Override point for customization after application launch.

    // Add the view controller's view to the window and display.
    window = [[UIWindow alloc] initWithFrame: [[UIScreen mainScreen] bounds]];
//    CCEAGLView *eaglView = [CCEAGLView viewWithFrame: [window bounds]
//                                     pixelFormat: (NSString*)cocos2d::GLViewImpl::_pixelFormat
//                                     depthFormat: cocos2d::GLViewImpl::_depthFormat
//                              preserveBackbuffer: NO
//                                      sharegroup: nil
//                                   multiSampling: NO
//                                 numberOfSamples: 0 ];

//    [eaglView setMultipleTouchEnabled:YES];
    
    // Use RootViewController manage CCEAGLView
    viewController = [[ViewController alloc] initWithNibName:nil bundle:nil];
    viewController.wantsFullScreenLayout = YES;
//    viewController.view = eaglView;

    // Set RootViewController to window
    if ( [[UIDevice currentDevice].systemVersion floatValue] < 6.0)
    {
        // warning: addSubView doesn't work on iOS6
        [window addSubview: viewController.view];
    }
    else
    {
        // use this method on ios6
        [window setRootViewController:viewController];
    }
    [window makeKeyAndVisible];
    [window setBackgroundColor:[UIColor redColor]];
    
//    [GameLuaOCBridge setRootView:viewController];
//    [GameLuaOCBridge setNotification:nullptr];

//    BaseSdk *basesdk = s_projectMgr->GetSdk();
//    if (nil != basesdk) { /** [basesdk SDKInit]; **/ }
    
    [[UIApplication sharedApplication] setStatusBarHidden: YES];
    [UIApplication sharedApplication].idleTimerDisabled=YES;

    // IMPORTANT: Setting the GLView should be done after creating the RootViewController
//    cocos2d::GLView *glview = cocos2d::GLViewImpl::createWithEAGLView(eaglView);
//    cocos2d::Director::getInstance()->setOpenGLView(glview);

    app->run();
    
    
    
    
    return YES;
}


- (void)application:(UIApplication *)application didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken
{
}

- (void)application:(UIApplication *)application didFailToRegisterForRemoteNotificationsWithError:(NSError *)error
{
}

- (void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo{
}

- (void)application:(UIApplication *)application didReceiveLocalNotification:(UILocalNotification *)notification
{
    
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
    // Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
//    cocos2d::Application::getInstance()->applicationWillEnterForeground();
}


- (void)applicationWillTerminate:(UIApplication *)application {
    // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
}

- (BOOL)application:(UIApplication *)application handleOpenURL:(NSURL *)url
{
    return  YES;//[WXApi handleOpenURL:url delegate:self];
}

//-(void) onReq:(BaseReq*)req
//{
//    if([req isKindOfClass:[GetMessageFromWXReq class]]) {
//        NSLog(@"微信请求App提供内容，App要调用sendResp:GetMessageFromWXResp返回给微信");
//        
//    } else if([req isKindOfClass:[ShowMessageFromWXReq class]]) {
//        NSLog(@"微信请求App提供内容，显示微信传过来的内容");
//        
//    } else if([req isKindOfClass:[LaunchFromWXReq class]]) {
//        NSLog(@"这是从微信启动的消息");
//    }
//}

//-(void) onResp:(BaseResp*)resp
//{
//    if([resp isKindOfClass:[SendMessageToWXResp class]])
//    {
//        NSLog(@"errcode:%d", resp.errCode);
//        NSString *code = [NSString stringWithFormat:@"%d", resp.errCode];
//        NSDictionary* detailDic = [NSDictionary dictionaryWithObjectsAndKeys:
//                                   @"share", @"type",
//                                   code, @"code", nil];
//        
//        BOOL isYes = [NSJSONSerialization isValidJSONObject:detailDic];
//        if (!isYes) {
//            NSLog(@"JSON数据生成失败，请检查数据格式");
//            return;
//        }
//        NSData * jsonData = [NSJSONSerialization dataWithJSONObject:detailDic options:0 error:nil];
//        NSString * myString = [[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding];
//        
//        const char* jsonStr = [myString UTF8String];
//        cocos2d::LuaEngine::getInstance()->executeGlobalFunction("sendWxRetCode", jsonStr);
//        return;
//        
//    } else if([resp isKindOfClass:[SendAuthResp class]]) {
//        SendAuthResp *aresp = (SendAuthResp *)resp;
//        NSString *code = aresp.code;
//        NSLog(@"onResp SendAuthResp errcode:%d", resp.errCode);
//        NSLog(@"onResp SendAuthResp code is:%@", code);
//        //ERR_OK = 0(用户同意)
//        //ERR_AUTH_DENIED = -4（用户拒绝授权）
//        //ERR_USER_CANCEL = -2（用户取消）
//        if (resp.errCode == 0) {
//            BaseSdk *sdk = s_projectMgr->GetSdk();
//            NSString* appid = [sdk GetWXAppID];
//            NSDictionary* detailDic = [NSDictionary dictionaryWithObjectsAndKeys:
//                                       code, @"code",
//                                       @"wx", @"plat",
//                                       appid, @"appid", nil];
//            
//            BOOL isYes = [NSJSONSerialization isValidJSONObject:detailDic];
//            if (!isYes) {
//                NSLog(@"JSON数据生成失败，请检查数据格式");
//                return;
//            }
//            NSData * jsonData = [NSJSONSerialization dataWithJSONObject:detailDic options:0 error:nil];
//            NSString * myString = [[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding];
//
//            const char* jsonStr = [myString UTF8String];
//            cocos2d::LuaEngine::getInstance()->executeGlobalFunction("SDKLoginCallback", jsonStr);
//            return;
//        }
//        NSString *tips = [[NSString alloc] initWithString:[NSString stringWithFormat:@"登陆错误%d", resp.errCode]];
//        if (resp.errCode == -2) {
//            tips = @"取消登陆";
//            return;
//        }
//        if (resp.errCode == -4) {
//            tips = @"拒绝授权登陆";
//        }
//        UIAlertView * alert = [[UIAlertView alloc] initWithTitle:@"提示"
//                                                         message:tips
//                                                        delegate:self
//                                               cancelButtonTitle:nil
//                                               otherButtonTitles:@"确定", nil];
//        [alert show];
//    } else if([resp isKindOfClass:[PayResp class]]) {
//        PayResp *response = (PayResp*)resp;
//        //
//        NSLog(@"onResp PayResp errcode:%d", resp.errCode);
//        NSLog(@"onResp PayResp returnKey:%@", response.returnKey);
//    }
//}


- (UIInterfaceOrientationMask)supportedInterfaceOrientationsForWindow:(nullable UIWindow *)window
{
    return UIInterfaceOrientationMaskAll;
}

- (void)applicationWillResignActive:(UIApplication *)application {
    /*
     Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
     Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
     */
//    cocos2d::Director::getInstance()->pause();
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
    /*
     Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
     */
    // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
//    cocos2d::Director::getInstance()->resume();
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
    /*
     Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
     If your application supports background execution, called instead of applicationWillTerminate: when the user quits.
     */
//    cocos2d::Application::getInstance()->applicationDidEnterBackground();
}


- (BOOL)application:(UIApplication *)application
            openURL:(NSURL *)url
  sourceApplication:(NSString *)sourceApplication
         annotation:(id)annotation
{
//    BaseSdk *sdk = s_projectMgr->GetSdk();
//    [sdk SDKHandleOpenURL:url :self];
    return YES;
}

// NOTE: 9.0以后使用新API接口
- (BOOL)application:(UIApplication *)app openURL:(NSURL *)url options:(NSDictionary<NSString*, id> *)options
{
//    BaseSdk *sdk = s_projectMgr->GetSdk();
//    [sdk SDKHandleOpenURL:url :self];
    return YES;
}


#pragma mark -
#pragma mark Memory management

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application {
    /*
     Free up as much memory as possible by purging cached data objects that can be recreated (or reloaded from disk) later.
     */
//     cocos2d::Director::getInstance()->purgeCachedData();
}


//- (void)dealloc {
//    [super dealloc];
//}


@end

