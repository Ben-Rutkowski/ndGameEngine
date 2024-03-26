#import "cocoa_interface.hpp"
#import "AppDelegate.h"

#include <iostream>

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

void killCocoa() {
    NSLog(@"Kill");
    [NSApp terminate:nil];
}


// ================ Callback ================
void nullCallback(void*, unsigned int)              {std::cout << "nullCallback" << std::endl;}
void nullCallbackVI2(void*, unsigned int, int, int) {std::cout << "nullCallbackVI2" << std::endl;}

void*                 EVENT_MANAGER_PTR  = nullptr;
CocoaEventCallBack    COCOA_CALLBACK     = nullCallback;
CocoaEventCallBackVI2 COCOA_CALLBACK_VI2 = nullCallbackVI2;
  
void setEventManagerCocoa(void* event_manager_ptr) {
    EVENT_MANAGER_PTR = event_manager_ptr;
    // NSLog(@"Setting Event Manager");
}

void setEventCallbackCocoa(CocoaEventCallBack callback) {
    COCOA_CALLBACK = callback;
    // NSLog(@"Setting Callback");
}

void setEventCallbackCocoaVI2(CocoaEventCallBackVI2 callback) {
    COCOA_CALLBACK_VI2 = callback;
    // NSLog(@"Setting Callback VI2");
}
