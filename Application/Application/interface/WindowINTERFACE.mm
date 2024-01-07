#import "cocoa_interface.hpp"
#import "WindowDelegate.h"
#import "RenderSwitcher.h"
#import "ndView.h"

ndWindow::ndWindow(int width, int height, const char* title) {    
    NSLog(@"Building Window");
    @autoreleasepool {
//        --- Winodw ---
        NSRect frame = NSMakeRect(0, 0, width, height);
        NSWindowStyleMask style = NSWindowStyleMaskClosable | NSWindowStyleMaskTitled | NSWindowStyleMaskResizable;
        NSWindow* window = [[NSWindow alloc] initWithContentRect:frame
                                                       styleMask:style
                                                         backing:NSBackingStoreBuffered
                                                           defer:NO];
        
        [window setTitle:[NSString stringWithUTF8String:title]];
        [window center];
        
//        --- Window Delegate ---
        WindowDelegate* window_delegate = [[WindowDelegate alloc] init];
        [window setDelegate:window_delegate];
        
//        --- ndView ---
        id<MTLDevice> device = MTLCreateSystemDefaultDevice();
        ndView* nd_view      = [[ndView alloc] initWithFrame:frame
                                                      device:device];
        [window.contentView addSubview:nd_view];
        
//        --- RenderSwitcher ---
        RenderSwitcher* render_switcher = [[RenderSwitcher alloc]
                                           initWithMTLDevice:device
                                           metalLayer:(CAMetalLayer*)nd_view.layer];
        [nd_view setRenderSwitcher:render_switcher];
    
//        --- Class Attributes ---
        window_COCOA         = (__bridge void*)[window retain];
        window_delegate_COCOA = (__bridge void*)[window_delegate retain];
        nd_view_COCOA         = (__bridge void*)[nd_view retain];
        render_switcher_COCOA = (__bridge void*)[render_switcher retain];
    }
}


// ================ Window Operations ================
bool ndWindow::shouldClose() {
    WindowDelegate* window_delegate = (WindowDelegate*)window_delegate_COCOA;
    return window_delegate.ndclose_window;
}

void ndWindow::showWindow() {
    NSWindow* window = (NSWindow*)window_COCOA;
    [window makeKeyAndOrderFront:nil];
}


// ================ Draw ================
void ndWindow::drawView() {
    ndView* nd_view = (ndView*)nd_view_COCOA;
    [nd_view draw];
}


// ================ Renderer ================
ndRoutine ndWindow::createDrawRoutine(ndDrawRoutineKind draw_routine_kind) {
    RenderSwitcher* render_switcher = (RenderSwitcher*)render_switcher_COCOA;
    NSUInteger index = [render_switcher createDrawRoutine:draw_routine_kind];
    id draw_routine = [render_switcher getDrawRoutineAtIndex:index];
    return ndRoutine(draw_routine, index);
}

void ndWindow::bindRoutine(ndRoutine routine) {
    RenderSwitcher* render_switcher = (RenderSwitcher*)render_switcher_COCOA;
    NSUInteger index = (NSUInteger)routine.index();
    [render_switcher bindRoutine:index];
}

void ndWindow::armRoutine() {
    RenderSwitcher* render_switcher = (RenderSwitcher*)render_switcher_COCOA;
    [render_switcher armRoutine];
}

//void ndWindow::configureRoutine() {
//    RenderSwitcher* render_switcher = (RenderSwitcher*)render_switcher_COCOA;
//    [render_switcher configureRoutine];
//}
