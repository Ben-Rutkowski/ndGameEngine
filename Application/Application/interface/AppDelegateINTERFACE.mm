#import "cocoa_interface.hpp"
#import "AppDelegate.h"

int initCocoa() {
    @autoreleasepool {
        AppDelegate* app_delegate = [[AppDelegate alloc] init];
        if (app_delegate == nil) {
            NSLog(@"Failed to create ndAppDelegate");
            return -1;
        }
        
        [NSApplication sharedApplication];
        [NSApp setDelegate:app_delegate];
        
        if (![[NSRunningApplication currentApplication] isFinishedLaunching]) {
            NSLog(@"Starting Cocoa Run Loop");
            [NSApp run];
        }

        CURRENT_APPLICATION_DELEGATE_COCOA = (__bridge void*)[app_delegate retain];
    }
    
    return 0;
}

void pollEventsCocoa() {
    @autoreleasepool {
        while (true) {
            NSEvent* event = [NSApp nextEventMatchingMask:NSEventMaskAny
                                                untilDate:[NSDate distantPast]
                                                   inMode:NSDefaultRunLoopMode
                                                  dequeue:YES];
            
            if (event == nil) {
                break;
            }

            [NSApp sendEvent:event];
        }
    }
}
