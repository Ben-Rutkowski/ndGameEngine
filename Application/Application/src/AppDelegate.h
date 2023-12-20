#ifndef APP_DELEGATE_H
#define APP_DELEGATE_H

#import <AppKit/AppKit.h>

@interface AppDelegate : NSObject <NSApplicationDelegate>
- (void) applicationWillFinishLaunching:(NSNotification*)notification;
- (void) applicationDidFinishLaunching:(NSNotification*)notification;

- (BOOL) shouldMainLoopEnd;
@end

#endif
