#import "WindowDelegate.h"

@implementation WindowDelegate

- (nonnull instancetype) init {
    self = [super init];
    if (self != nil) {
        [self setNdclose_window:NO];
    }
    
    return self;
}

@end
