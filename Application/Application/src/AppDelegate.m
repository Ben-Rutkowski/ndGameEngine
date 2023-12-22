#import "AppDelegate.h"
#import "WindowDelegate.h"

@implementation AppDelegate

- (void) applicationWillFinishLaunching:(NSNotification*)notification {
    NSLog(@"Application Will Finish Launching");
    [self createMainMenu];
}

- (void) applicationDidFinishLaunching:(NSNotification*)notification {
    NSLog(@"Application Finished Launching");
    
    @autoreleasepool {
        [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
        [NSApp activate];
        
        NSEvent* event = [NSEvent otherEventWithType:NSEventTypeApplicationDefined
                                            location:NSMakePoint(0, 0)
                                       modifierFlags:0
                                           timestamp:0
                                        windowNumber:0
                                             context:nil
                                             subtype:0
                                               data1:0
                                               data2:0];
        
        [NSApp postEvent:event atStart:YES];
    }
    
    NSLog(@"Stopping Cocoa Run Loop");
    [NSApp stop:nil];
}

// ================ Events ================
- (void) quitEvent {
    NSLog(@"Terminating App");
    [NSApp terminate:nil];
}

// ================ Creating Menus ================
- (void) createMainMenu {
    @autoreleasepool {
        NSLog(@"Creating Main Menu");
        
        NSMenu* main_menu = [[NSMenu alloc] init];
        [main_menu addItem:[self createAppMenuItem]];
        
        [NSApp setMainMenu:main_menu];
    }
}

- (NSMenuItem*) createAppMenuItem {
    NSMenuItem* app_menu_tab = [[NSMenuItem alloc] init];
    
    @autoreleasepool {
        NSMenu* app_menu = [[NSMenu alloc] init];
        
//        Quit Item
        NSMenuItem* quit_item = [[NSMenuItem alloc] initWithTitle:@"Quit"
                                                           action:@selector(quitEvent)
                                                   keyEquivalent:@"q"];
        [quit_item setKeyEquivalentModifierMask:NSEventModifierFlagCommand];
        
//        Creating Menu
        [app_menu addItem:quit_item];
        
//        Creating Menu Tab
        [app_menu_tab setSubmenu:app_menu];
    }
    
    return [app_menu_tab autorelease];
}

@end
