#import "DrawRoutines.h"

@implementation StaticShapeRoutine
{
//    --- Subroutines ---
    StaticShapeSubroutine* _static_shape_subroutine;
    
//    --- Buffers ---
    id<MTLBuffer> _vertex_data_buffer;
    NSUInteger    _vertex_count;
}

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

- (nonnull StaticShape_VertexType*) createVertexBufferWithVertexNumber:(NSUInteger)number {
    _vertex_data_buffer = [self newSharedBufferWithLength:number*sizeof(StaticShape_VertexType)];
    
    [_static_shape_subroutine linkVertexDataBuffer:_vertex_data_buffer];
    return _vertex_data_buffer.contents;
}

- (void)drawInDrawable:(nonnull id<CAMetalDrawable>)drawable inCommandBuffer:(nonnull id<MTLCommandBuffer>)command_buffer {
    [_static_shape_subroutine encodeSubroutineInBuffer:command_buffer inTexture:drawable.texture];
    [command_buffer presentDrawable:drawable];
    [command_buffer commit];
}

- (void)reloadSharedData {
}

@end
