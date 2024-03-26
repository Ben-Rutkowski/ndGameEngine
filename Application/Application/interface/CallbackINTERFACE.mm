#define __IN_COCOA__
#import "cocoa_interface.hpp"

#include <iostream>

// ================ Callback ================
@implementation ndCallBackCenter

+ (void) callback:(CocoaCallback_E)type {
    COCOA_CALLBACK(EVENT_MANAGER_PTR, type);
}

@end
