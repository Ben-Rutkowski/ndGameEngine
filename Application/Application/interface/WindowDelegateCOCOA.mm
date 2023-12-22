#import "cocoa_interface.hpp"
#import "WindowDelegate.h"

ndWindow::ndWindow(int width, int height, const char* title) {
    is_open = true;
    
    NSLog(@"Building Window");
    @autoreleasepool {
//        Winodw
        NSRect frame = NSMakeRect(0, 0, width, height);
        NSWindowStyleMask style = NSWindowStyleMaskClosable | NSWindowStyleMaskTitled | NSWindowStyleMaskResizable;
        NSWindow* window = [[NSWindow alloc] initWithContentRect:frame
                                                       styleMask:style
                                                         backing:NSBackingStoreBuffered
                                                           defer:NO];
        
    
//        Window Delegate
        WindowDelegate* window_delegate = [[WindowDelegate alloc] init];
        [window setDelegate:window_delegate];
    
//        Initialization
        [window setTitle:[NSString stringWithUTF8String:title]];
        [window makeKeyAndOrderFront:nil];
        [window center];
    
//        Class Attributes
        WindowCOCOA = (__bridge void*)[window retain];
        WindowDelegateCOCOA = (__bridge void*)[window_delegate retain];
    }
}

bool ndWindow::shouldClose() {
    WindowDelegate* window_delegate = (WindowDelegate*)WindowDelegateCOCOA;
    return window_delegate.ndclose_window;
}

void ndWindow::debug() {
    NSLog(@"Debugging ndWindow");
    NSWindow* window = (__bridge NSWindow*)WindowCOCOA;
    
    NSLog(@"Window: %lu", [window retainCount]);
//    [NSApp run];
}
