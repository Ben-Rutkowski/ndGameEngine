#import "Renderer.h"
#import "DrawRoutines.h"

@implementation Renderer
{
    id<MTLDevice> _device;
    id<MTLCommandQueue> _command_queue;
    
    BasicTriangleDrawRoutine* _basic_triangle_routine;
}

- (nonnull instancetype) initWithMetalKitView:(MTKView*)mtk_view {
    self = [super init];
    if (self) {
        _device = mtk_view.device;
        _command_queue = [_device newCommandQueue];
        
        NSError* error;
        NSURL* baseurl = [NSURL fileURLWithPath:@"/Users/benjaminrutkowski/"];
        NSURL* url     = [NSURL URLWithString:@"Projects/ndGameEngine/Application/compiled/shader_lib/MetalLibrary.metallib"
                                relativeToURL:baseurl];
        id<MTLLibrary> library = [_device newLibraryWithURL:url error:&error];
        NSAssert(library, @"Failed to find Metal Library", error);
        
        _basic_triangle_routine = [[BasicTriangleDrawRoutine alloc] initWithMTKView:mtk_view MTLLibrary:library];
    }
    
    return self;
}

- (void)mtkView:(MTKView*)mtk_view drawableSizeWillChange:(CGSize)size {
    NSLog(@"Resizing MTKVeiw");
}

- (void)drawInMTKView:(MTKView*)mtk_view {
    [_basic_triangle_routine commitWithMTKView:mtk_view CommandQueue:_command_queue];
}

@end
