#import "cocoa_interface.hpp"
#import "WindowDelegate.h"
#import "Renderer.h"
#import "ndView.h"

ndWindow::ndWindow(int width, int height, const char* title) {    
    NSLog(@"Building Window");
    @autoreleasepool {
//        Winodw
        NSRect frame = NSMakeRect(0, 0, width, height);
        NSWindowStyleMask style = NSWindowStyleMaskClosable | NSWindowStyleMaskTitled | NSWindowStyleMaskResizable;
        NSWindow* window = [[NSWindow alloc] initWithContentRect:frame
                                                       styleMask:style
                                                         backing:NSBackingStoreBuffered
                                                           defer:NO];
        
        [window setTitle:[NSString stringWithUTF8String:title]];
        [window center];
        
//        Window Delegate
        WindowDelegate* window_delegate = [[WindowDelegate alloc] init];
        [window setDelegate:window_delegate];
        
//        ndView
        id<MTLDevice> device = MTLCreateSystemDefaultDevice();
        ndView* nd_view = [[ndView alloc] initWithFrame:frame
                                                 device:device];
        [window.contentView addSubview:nd_view];
    
//        Class Attributes
        WindowCOCOA = (__bridge void*)[window retain];
        WindowDelegateCOCOA = (__bridge void*)[window_delegate retain];
        ndViewCOCOA = (__bridge void*)[nd_view retain];
    }
}

// ================ Interface ================
bool ndWindow::shouldClose() {
    WindowDelegate* window_delegate = (WindowDelegate*)WindowDelegateCOCOA;
    return window_delegate.ndclose_window;
}

void ndWindow::setClearColor(double r, double g, double b, double a) {
    
}

void ndWindow::showWindow() {
    NSWindow* window = (NSWindow*)WindowCOCOA;
    [window makeKeyAndOrderFront:nil];
}

void ndWindow::drawView() {
    ndView* nd_view = (ndView*)ndViewCOCOA;
    [nd_view render];
}

// ================ Debugging ================
void ndWindow::debug() {
    NSLog(@"Debugging ndWindow");
    NSWindow* window = (__bridge NSWindow*)WindowCOCOA;
    
    NSLog(@"Window: %lu", [window retainCount]);
}
