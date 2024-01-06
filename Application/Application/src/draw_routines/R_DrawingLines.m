#import "DrawRoutines.h"

@implementation DrawingLines
{
    LineSubroutine* _draw_line_subroutine;
    DynamicBuffer*  _buffer;
}


// ==== Configure ====
- (instancetype) initWithDevice:(id<MTLDevice>)device
                        library:(id<MTLLibrary>)library
{
    self = [super initWithDevice:device];
    if (self) {
        _draw_line_subroutine = [[LineSubroutine alloc] initWithDevice:device
                                                               library:library];
    }
    return self;
}

- (void) configureWithDrawablePixelFormat:(MTLPixelFormat)pixel_format {
    [_draw_line_subroutine configureWithDrawablePixelFormat:pixel_format];
}

- (void) createBufferWithVertexCount:(NSUInteger)count {
    _buffer = [self newDynamicBufferWithVertexSize:sizeof(Line_TriagVtype)
                                       vertexCount:count
                                       storageMode:MTLResourceStorageModeShared];
    [_draw_line_subroutine linkBuffer:_buffer];
}


// ==== Resources ====
- (void) bindBuffer:(NSUInteger)buffer_index {}

- (id<MTLBuffer>) writeBufferOpen {
    return [_buffer writeOpen];
}

- (void) writeBufferClose {
    @autoreleasepool {
        id<MTLCommandBuffer> blit_command_buffer = [self getBlitCommandBuffer];
        [_buffer writeCloseInBlitCommandBuffer:blit_command_buffer];
        [blit_command_buffer commit];
    }
}


// ==== Draw ====
- (void) drawInDrawable:(id<CAMetalDrawable>)drawable
        inCommandBuffer:(id<MTLCommandBuffer>)command_buffer
{
    [_draw_line_subroutine encodeSubroutineInBuffer:command_buffer
                                          inTexture:drawable.texture];
}

- (void)predrawOpenInBuffers {
    [_buffer predrawOpen];
}

- (void)predrawCloseInBuffers {
    [_buffer predrawClose];
}

- (void)drawCompletedInBuffers {
    [_buffer drawCompleted];
}

@end
