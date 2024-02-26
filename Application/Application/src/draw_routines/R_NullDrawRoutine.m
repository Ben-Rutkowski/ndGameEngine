#import "DrawRoutines.h"

@implementation NullDrawRoutine
- (nonnull instancetype) initWithDevice:(nonnull id<MTLDevice>)device
                           commandQueue:(nonnull id<MTLCommandQueue>)command_queue
                                library:(nonnull id<MTLLibrary>)library
                            pixelFormat:(MTLPixelFormat)pixel_format
{
    self = [super init];
    return self;
}

- (void) drawInDrawable:(nonnull id<CAMetalDrawable>)drawable
        inCommandBuffer:(nonnull id<MTLCommandBuffer>)command_buffer
{
//    NSLog(@"-- Draw in Null Draw Routine --");
}
@end
