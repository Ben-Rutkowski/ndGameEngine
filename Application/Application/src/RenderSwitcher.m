#import "RenderSwitcher.h"
#import "DrawRoutines.h"

@implementation RenderSwitcher
{
    id<MTLDevice>       _device;
    id<MTLCommandQueue> _command_queue;
    id<MTLLibrary>      _library;
    
    NSMutableArray* _loaded_draw_routines;
    
    StaticShapeRoutine* _static_shape_routine;
    StaticNiceLineRoutine* _static_nice_line_routine;
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
        
        _static_shape_routine = [[StaticShapeRoutine alloc] initWithDevice:_device
                                                                   library:_library];
        
        _static_nice_line_routine = [[StaticNiceLineRoutine alloc] initWithDevice:_device
                                                                          library:_library];
    
//        Debugging
        StaticShape_VertexType* vertices = [_static_shape_routine createVertexBufferWithVertexNumber:3];
        
        vertices[0].position = (vector_float2){ -0.5, -0.5 };
        vertices[1].position = (vector_float2){  0.5, -0.5 };
        vertices[2].position = (vector_float2){  0.0,  0.5 };
        
        vertices[0].color = (vector_float4){ 0.0, 0.5, 0.7, 1.0 };
        vertices[1].color = (vector_float4){ 0.3, 0.5, 0.7, 1.0 };
        vertices[2].color = (vector_float4){ 0.5, 0.5, 0.7, 1.0 };
        
        NSLog(@"probe end");
        
        [_static_shape_routine configureWithDrawablePixelFormat:layer.pixelFormat];
        
//        Drawing Line
        [self createDefaultLine];
        [_static_nice_line_routine configureWithDrawablePixelFormat:layer.pixelFormat];
        
        
    }
    
    return self;
}

- (unsigned int) initDrawRoutine:(unsigned int)draw_routine_kind {
    return 500;
}

- (void) drawInMetalLayer:(CAMetalLayer*)metal_layer {
    @autoreleasepool {
        id<CAMetalDrawable> current_drawable = [metal_layer nextDrawable];
        if (current_drawable ==  nil) {
            NSLog(@"No Drawable");
            return;
        }
        
        id<MTLCommandBuffer> command_buffer = [_command_queue commandBuffer];
        
//        [_static_shape_routine drawInDrawable:current_drawable
//                              inCommandBuffer:command_buffer];
        [_static_nice_line_routine drawInDrawable:current_drawable
                                  inCommandBuffer:command_buffer];
    }
}

- (void) createDefaultLine {
    NiceLine_VertexType* verts = [_static_nice_line_routine createVertexBufferWithVertexNumber:6];
    
//    Body
    verts[0].position = (vector_float2){ -0.5, -0.5 };
    verts[1].position = (vector_float2){  0.5, -0.5 };
    verts[2].position = (vector_float2){ -0.5,  0.5 };
    verts[0].uv = (vector_float2){ -1.0, 0.0 };
    verts[1].uv = (vector_float2){  1.0, 0.0 };
    verts[2].uv = (vector_float2){  1.0, 1.0 };
    verts[0].color = (vector_float4){ 1.0, 0.0, 0.7, 1.0 };
    verts[1].color = (vector_float4){ 1.0, 0.0, 0.7, 1.0 };
    verts[2].color = (vector_float4){ 1.0, 1.0, 0.7, 1.0 };
    
    verts[3].position = (vector_float2){  0.5, -0.5 };
    verts[4].position = (vector_float2){  0.5,  0.5 };
    verts[5].position = (vector_float2){ -0.5,  0.5 };
    verts[3].uv = (vector_float2){  1.0, 0.0 };
    verts[4].uv = (vector_float2){  1.0, 1.0 };
    verts[5].uv = (vector_float2){ -1.0, 1.0 };
    verts[3].color = (vector_float4){ 0.3, 0.5, 0.7, 1.0 };
    verts[4].color = (vector_float4){ 0.3, 0.5, 0.7, 1.0 };
    verts[5].color = (vector_float4){ 0.3, 0.5, 0.7, 1.0 };
}

@end
