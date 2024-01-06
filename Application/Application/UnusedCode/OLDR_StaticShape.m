#import "DrawRoutines.h"

@implementation StaticShapeRoutine
{
//    --- Subroutines ---
    StaticShapeSubroutine* _draw_subroutine;
    DynamicBuffer*         _shape_vertices;
}

// ==== Configuring ====
- (instancetype) initWithDeviceOLD:(nonnull id<MTLDevice>)device
                                library:(nonnull id<MTLLibrary>)library
{
    self = [super initWithDevice:device];
    if (self) {
        _draw_subroutine = [[StaticShapeSubroutine alloc] initWithDevice:device
                                                                 library:library];
    }
    return self;
}

- (void) configureWithDrawablePixelFormatOLD:(MTLPixelFormat)pixel_format {
    [_draw_subroutine configureWithDrawablePixelFormatOLD:pixel_format];
}

- (void) createBufferWithVertexCountOLD:(NSUInteger)count {
    _shape_vertices = [self newDynamicBufferWithVertexSizeOLD:sizeof(StaticShapeVtype)
                                             vertexCount:count
                                             storageMode:MTLResourceStorageModeShared];
    [_draw_subroutine linkBuffer:_shape_vertices];
}


// ==== Resources ====
- (void) bindBufferOLD:(NSUInteger)buffer_index {}

- (id<MTLBuffer>) writeBufferOpenOLD {
    return [_shape_vertices writeOpen];
}

- (void) writeBufferCloseOLD {
    @autoreleasepool {
        id<MTLCommandBuffer> blit_command_buffer = [self getBlitCommandBuffer];
        [_shape_vertices writeCloseInBlitCommandBuffer:blit_command_buffer];
        [blit_command_buffer commit];
    }
}


// ==== Drawing ====
- (void) drawInDrawable:(id<CAMetalDrawable>)drawable
        inCommandBuffer:(id<MTLCommandBuffer>)command_buffer
{
    [_draw_subroutine encodeSubroutineInBuffer:command_buffer
                                     inTexture:drawable.texture];
}

- (void) predrawOpenInBuffersOLD {
    [_shape_vertices predrawOpen];
}

- (void) predrawCloseInBuffersOLD {
    [_shape_vertices predrawClose];
}

- (void) drawCompletedInBuffersOLD {
    [_shape_vertices drawCompleted];
}

@end
