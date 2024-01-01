#define __IN_COCOA__
#import "RenderSwitcher.h"
#import "DrawRoutines.h"
#import "cocoa_interface.hpp"

@implementation RenderSwitcher
{
    id<MTLDevice>       _device;
    id<MTLCommandQueue> _command_queue;
    id<MTLLibrary>      _library;
    
    NSMutableArray* _loaded_draw_routines;
    
//    --- Debugging ---
    StaticShapeRoutine*    _static_shape_routine;
}

- (nonnull instancetype) initWithMTLDevice:(id<MTLDevice>)device
                                metalLayer:(nonnull CAMetalLayer *)layer
{
    self = [super init];
    if (self) {
        _device        = [device retain];
        _command_queue = [_device newCommandQueue];
        
        _loaded_draw_routines = [[NSMutableArray alloc] init];
        
        NSError* error;
        NSURL* baseurl = [NSURL fileURLWithPath:@"/Users/benjaminrutkowski/"];
        NSURL* url     = [NSURL URLWithString:@"Projects/ndGameEngine/Application/compiled/shader_lib/MetalLibrary.metallib"
                                relativeToURL:baseurl];
        _library = [_device newLibraryWithURL:url error:&error];
        NSAssert(_library, @"Failed to find Metal Library", error);
        
        [self debug:layer];
    }
    
    return self;
}

- (void) drawInMetalLayer:(CAMetalLayer*)metal_layer {
    @autoreleasepool {
        id<CAMetalDrawable> current_drawable = [metal_layer nextDrawable];
        if (current_drawable ==  nil) {
            return;
        }
        
        id<MTLCommandBuffer> command_buffer = [_command_queue commandBuffer];
        
        [_static_shape_routine drawInDrawable:current_drawable
                              inCommandBuffer:command_buffer];
    }
}

- (NSUInteger) initDrawRoutine:(NSUInteger)draw_routine_kind {
    switch (draw_routine_kind) {
        case ndDrawRoutineKindDebug:
            NSLog(@"Should Init Draw Routine Debug");
            break;
        default: break;
    }
    
    return 500;
}


// ================ Debugging ================
- (void) debug:(CAMetalLayer*)layer {
    _static_shape_routine = [[StaticShapeRoutine alloc] initWithDevice:_device
                                                               library:_library];
    
    [_static_shape_routine createBufferWithVertexCount:3];
    StaticShape_VertexType* vertices = [_static_shape_routine getBuffer].contents;
    
    vertices[0].position = (vector_float2){ -0.5, -0.5 };
    vertices[1].position = (vector_float2){  0.5, -0.5 };
    vertices[2].position = (vector_float2){  0.0,  0.5 };
    
    vertices[0].color = (vector_float4){ 0.0, 0.5, 0.7, 1.0 };
    vertices[1].color = (vector_float4){ 0.3, 0.5, 0.7, 1.0 };
    vertices[2].color = (vector_float4){ 0.5, 0.5, 0.7, 1.0 };
    
    [_static_shape_routine configureWithDrawablePixelFormat:layer.pixelFormat];
}

@end
