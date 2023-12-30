#import "Renderer.h"
#import "DrawRoutines.h"

@implementation Renderer
{
    id<MTLDevice> _device;
    id<MTLCommandQueue> _command_queue;
    
//    BasicTriangleDrawRoutine* _basic_triangle_routine;
//    ParallelTriangleRotateDrawRoutine* _parallel_triangle_rotate_routine;
    BasicTriangleCustomViewDrawRoutine* _basic_triangle_custome_view_routine;
}

- (nonnull instancetype) initWithMTLDevice:(id<MTLDevice>)device drawablePixelFormat:(MTLPixelFormat)pixel_format {
    self = [super init];
    if (self) {
        _device = device;
        _command_queue = [_device newCommandQueue];
        
        NSError* error;
        NSURL* baseurl = [NSURL fileURLWithPath:@"/Users/benjaminrutkowski/"];
        NSURL* url     = [NSURL URLWithString:@"Projects/ndGameEngine/Application/compiled/shader_lib/MetalLibrary.metallib"
                                relativeToURL:baseurl];
        id<MTLLibrary> library = [_device newLibraryWithURL:url error:&error];
        NSAssert(library, @"Failed to find Metal Library", error);
        
        _basic_triangle_custome_view_routine = [[BasicTriangleCustomViewDrawRoutine alloc] initWithDevice:device
                                                                                               MTLLibrary:library 
                                                                                      drawablePixelFormat:pixel_format];
    }
    
    return self;
}

- (void) drawInMetalLayer:(CAMetalLayer*)metal_layer {
    [_basic_triangle_custome_view_routine commitIntoMetalLayer:metal_layer commandQueue:_command_queue];
}

@end
