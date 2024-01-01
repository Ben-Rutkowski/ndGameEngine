#import "DrawRoutines.h"

@implementation StaticShapeRoutine
{
//    --- Subroutines ---
    StaticShapeSubroutine* _static_shape_subroutine;
    
//    --- Resources ---
    id<MTLBuffer> _vertex_data_buffer;
    NSUInteger    _vertex_count;
}

// ==== Configure ====
- (nonnull instancetype)initWithDevice:(nonnull id<MTLDevice>)device
                               library:(nonnull id<MTLLibrary>)library
{
    self = [super initWithDevice:device];
    if (self) {
        _static_shape_subroutine = [[StaticShapeSubroutine alloc] initWithDevice:device
                                                                         library:library];
    }
    return self;
}

- (void) configureWithDrawablePixelFormat:(MTLPixelFormat)pixel_format {
    [_static_shape_subroutine configureWithDrawablePixelFormat:pixel_format];
}


// ==== Resources ====
- (void)bindBuffer:(NSUInteger)buffer_index {}
- (void)createBufferWithVertexCount:(NSUInteger)count {
    _vertex_data_buffer = [self newSharedBufferWithLength:count*sizeof(StaticShape_VertexType)];
    [_static_shape_subroutine linkBuffer:_vertex_data_buffer vertexCount:count];
}

- (nonnull id<MTLBuffer>)getBuffer {
    return _vertex_data_buffer;
}


// ==== Drawing ====
- (void)drawInDrawable:(nonnull id<CAMetalDrawable>)drawable inCommandBuffer:(nonnull id<MTLCommandBuffer>)command_buffer {
    [_static_shape_subroutine encodeSubroutineInBuffer:command_buffer inTexture:drawable.texture];
    [command_buffer presentDrawable:drawable];
    [command_buffer commit];
}

- (void)reloadSharedData {
}

@end
