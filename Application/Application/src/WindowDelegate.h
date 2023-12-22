#ifndef WINDOW_DELEGATE_COCOA_H
#define WINDOW_DELEGATE_COCOA_H

#import <AppKit/AppKit.h>

@interface WindowDelegate : NSObject<NSWindowDelegate>

@property BOOL ndclose_window;

- (nonnull instancetype) init;

@end

#endif
