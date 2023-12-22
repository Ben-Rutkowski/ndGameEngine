#import "OLDWindowDelegate.h"

@implementation WindowDelegate

- (instancetype) init {
    self = [super init];
    if (self) {
        [self setClose_window:NO];
    }
    
    return self;
}

- (void) windowWillClose:(NSNotification*)notification {
    NSLog(@"Window CLOSED");
}

- (BOOL) windowShouldClose:(NSWindow*)sender {
    NSLog(@"Setting CLOSE");
    [self setClose_window:YES];
    
    return YES;
}

@end
