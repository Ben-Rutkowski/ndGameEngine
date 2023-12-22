#import "WindowDelegate.h"

@implementation WindowDelegate

- (nonnull instancetype) init {
    self = [super init];
    if (self != nil) {
        [self setNdclose_window:NO];
    }
    
    return self;
}

- (void) windowWillClose:(NSNotification*)notification {
    NSLog(@"Window Will Close");
    [self setNdclose_window:YES];
}

@end
