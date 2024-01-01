#import "DrawRoutines.h"

@implementation StaticNiceLineRoutine
{    
//    --- Subroutines ---
    StaticNiceLineSubroutine* _draw_line_subroutine;
    
//    --- Buffers ---
    id<MTLBuffer> _vertex_data_buffer;
    NSUInteger    _vertex_count;
}

- (nonnull instancetype)initWithDevice:(nonnull id<MTLDevice>)device 
                               library:(nonnull id<MTLLibrary>)library
{
    self = [super initWithDevice:device];
    if (self) {
        _draw_line_subroutine = [[StaticNiceLineSubroutine alloc] initWithDevice:device
                                                                         library:library];
    }
    
    return self;
}

- (void)configureWithDrawablePixelFormat:(MTLPixelFormat)pixel_format {
    [_draw_line_subroutine configureWithDrawablePixelFormat:pixel_format];
}

- (nonnull NiceLine_VertexType*)createVertexBufferWithVertexNumber:(NSUInteger)number {
    _vertex_count       = number;
    _vertex_data_buffer = [self newSharedBufferWithLength:_vertex_count*sizeof(NiceLine_VertexType)];
    
    [_draw_line_subroutine linkVertexDataBuffer:_vertex_data_buffer vertexCount:_vertex_count];
    return _vertex_data_buffer.contents;
}

- (void)drawInDrawable:(nonnull id<CAMetalDrawable>)drawable inCommandBuffer:(nonnull id<MTLCommandBuffer>)command_buffer {
    [_draw_line_subroutine encodeSubroutineInBuffer:command_buffer inTexture:drawable.texture];
    [command_buffer presentDrawable:drawable];
    [command_buffer commit];
}

- (void)reloadSharedData {
}

@end
