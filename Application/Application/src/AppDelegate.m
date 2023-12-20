#import "AppDelegate.h"
#import "ViewController.h"
#import "WindowDelegate.h"

@implementation AppDelegate
{
    NSWindow* _window;
    WindowDelegate* _window_delegate;
    ViewController* _view_controller;
}

- (void) applicationWillFinishLaunching:(NSNotification*)notification {
    NSLog(@"Application Will Finish Launching");
}

- (void) applicationDidFinishLaunching:(NSNotification*)notification {
    NSLog(@"Application Did Finish Launching");
    
    @autoreleasepool {
        NSRect frame = NSMakeRect(0, 0, 800, 600);
        NSWindowStyleMask style = NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskResizable;
        _window = [[NSWindow alloc] initWithContentRect:frame
                                              styleMask:style
                                                backing:NSBackingStoreBuffered
                                                  defer:NO];
        
        NSEvent* event = [NSEvent otherEventWithType:NSEventTypeApplicationDefined
                                            location:NSMakePoint(0,0)
                                       modifierFlags:0
                                           timestamp:0
                                        windowNumber:0
                                             context:nil
                                             subtype:0
                                               data1:0
                                               data2:0];
        
        [NSApp postEvent:event atStart:YES];
        
        _window_delegate = [[WindowDelegate alloc] init];
        [_window setDelegate:_window_delegate];
    }
    
    [_window center];
    
//    View Controller
    _view_controller = [[ViewController alloc] init];
    [_window.contentView addSubview:_view_controller.view];
    
    [NSApp activateIgnoringOtherApps:YES];
    [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
    [_window makeKeyAndOrderFront:nil];
    
    NSLog(@"Stopping Main Loop");
    [NSApp stop:nil];
}

- (BOOL) shouldMainLoopEnd {
    return _window_delegate.close_window;
}

@end
