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


// ================ Routine Interface ================
unsigned int ndWindow::createDrawRoutine(ndDrawRoutineKind draw_routine_kind) {
    RenderSwitcher* render_switcher = (RenderSwitcher*)render_switcher_COCOA;
    return (unsigned int)[render_switcher createDrawRoutine:draw_routine_kind];
}

void ndWindow::bindRoutine(unsigned int draw_routine_index) {
    RenderSwitcher* render_switcher = (RenderSwitcher*)render_switcher_COCOA;
    [render_switcher bindRoutine:draw_routine_index];
}

void ndWindow::configureRoutine() {
    RenderSwitcher* render_switcher = (RenderSwitcher*)render_switcher_COCOA;
    [render_switcher configureRoutine];
}

void ndWindow::armRoutine() {
    RenderSwitcher* render_switcher = (RenderSwitcher*)render_switcher_COCOA;
    [render_switcher armRoutine];
}

// ==== Resources ====
void ndWindow::bindBuffer(unsigned int buffer_index) {
    RenderSwitcher* render_switcher = (RenderSwitcher*)render_switcher_COCOA;
    [render_switcher bindBuffer:buffer_index];
}

void ndWindow::createBuffer(unsigned int vertex_count) {
    RenderSwitcher* render_switcher = (RenderSwitcher*)render_switcher_COCOA;
    [render_switcher createBufferWithVertexCount:vertex_count];
}

ndBuffer ndWindow::getBuffer() {
    RenderSwitcher* render_switcher = (RenderSwitcher*)render_switcher_COCOA;
    ResizableBuffer*  dynamic_buffer  = [render_switcher getBuffer];
    return ndBuffer(dynamic_buffer);
}
