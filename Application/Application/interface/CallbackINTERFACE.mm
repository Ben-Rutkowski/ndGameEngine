#define __IN_COCOA__
#import "cocoa_interface.hpp"

#include <iostream>

// ================ Callback ================
@implementation ndCallBackCenter

+ (void) callback:(CocoaCallback_E)type {
    COCOA_CALLBACK(EVENT_MANAGER_PTR, type);
}

+ (void) callback:(CocoaCallback_E)type
       withIntOne:(int)int_one
       withIntTwo:(int)int_two
{
    COCOA_CALLBACK_VI2(EVENT_MANAGER_PTR, type, int_one, int_two);
}

+ (void) debug {
    std::cout << EVENT_MANAGER_PTR << std::endl;
    std::cout << COCOA_CALLBACK << std::endl;
    COCOA_CALLBACK(EVENT_MANAGER_PTR, 0);
}

@end
