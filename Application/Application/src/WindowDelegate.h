#ifndef WINDOW_DELEGATE_H
#define WINDOW_DELEGATE_H

#import <AppKit/AppKit.h>

@interface WindowDelegate : NSObject<NSWindowDelegate>

@property BOOL close_window;

- (instancetype) init;
- (void) windowWillClose:(NSNotification*)notification;
- (BOOL) windowShouldClose:(NSWindow*)sender;
@end

#endif
