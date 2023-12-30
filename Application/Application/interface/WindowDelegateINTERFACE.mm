#import "cocoa_interface.hpp"
#import "WindowDelegate.h"
#import "Renderer.h"

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
        
//        MTKView
        MTKView* mtk_view = [[MTKView alloc] initWithFrame:frame
                                                    device:MTLCreateSystemDefaultDevice()];
        mtk_view.wantsLayer = YES;
        mtk_view.enableSetNeedsDisplay = NO;
        NSLog(@"Framerate: %li", mtk_view.preferredFramesPerSecond);
        mtk_view.autoresizingMask = NSViewWidthSizable | NSViewHeightSizable;
        
//        Renderer
        Renderer* renderer = [[Renderer alloc] initWithMetalKitView:mtk_view];
        if (!renderer) { NSLog(@"Renderer initialization failed"); }
        [renderer mtkView:mtk_view drawableSizeWillChange:mtk_view.drawableSize];
        [mtk_view setDelegate:renderer];
        [window.contentView addSubview:mtk_view];
        
    
//        Class Attributes
        WindowCOCOA = (__bridge void*)[window retain];
        WindowDelegateCOCOA = (__bridge void*)[window_delegate retain];
        MTKViewCOCOA = (__bridge void*)[mtk_view retain];
        MTKRendererCOCOA = (__bridge void*)[renderer retain];
    }
}

// ================ Interface ================
bool ndWindow::shouldClose() {
    WindowDelegate* window_delegate = (WindowDelegate*)WindowDelegateCOCOA;
    return window_delegate.ndclose_window;
}

void ndWindow::setClearColor(double r, double g, double b, double a) {
    MTKView* mtk_view = (MTKView*)MTKViewCOCOA;
    mtk_view.clearColor = MTLClearColorMake(r, g, b, a);
}

void ndWindow::showWindow() {
    NSWindow* window = (NSWindow*)WindowCOCOA;
    [window makeKeyAndOrderFront:nil];
}

void ndWindow::drawView() {
    MTKView* mtk_view = (MTKView*)MTKViewCOCOA;
    [mtk_view draw];
    
//    Renderer* renderer = (Renderer*)MTKRendererCOCOA;
//    [renderer debug];
}

// ================ Debugging ================
void ndWindow::debug() {
    NSLog(@"Debugging ndWindow");
    NSWindow* window = (__bridge NSWindow*)WindowCOCOA;
    
    NSLog(@"Window: %lu", [window retainCount]);
}
