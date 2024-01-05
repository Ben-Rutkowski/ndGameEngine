#import "DrawRoutines.h"

@implementation StaticShapeRoutine
{
//    --- Subroutines ---
    StaticShapeSubroutine* _draw_subroutine;
    DynamicBuffer*         _shape_vertices;
}

// ==== Configuring ====
- (instancetype) initWithDevice:(nonnull id<MTLDevice>)device
                                library:(nonnull id<MTLLibrary>)library
{
    self = [super initWithDevice:device];
    if (self) {
        _draw_subroutine = [[StaticShapeSubroutine alloc] initWithDevice:device
                                                                 library:library];
    }
    return self;
}

- (void) createBufferWithVertexCount:(NSUInteger)count {
    _shape_vertices = [self newDynamicBufferWithVertexSize:sizeof(StaticShape_VertexType)
                                             vertexCount:count
                                             storageMode:MTLResourceStorageModeShared];
    
    [_draw_subroutine linkBuffer:_shape_vertices];
}

- (void) configureWithDrawablePixelFormat:(MTLPixelFormat)pixel_format {
    [_draw_subroutine configureWithDrawablePixelFormat:pixel_format];
}


// ==== Resources ====
- (void) bindBuffer:(NSUInteger)buffer_index {}

- (id<MTLBuffer>) writeBufferOpen {
    return [_shape_vertices writeOpen];
}

- (void) writeBufferClose {
    id<MTLCommandBuffer> command_buffer = [self getBlitCommandBuffer];
    [_shape_vertices writeCloseInCommandBuffer:command_buffer];
    [command_buffer commit];
}


// ==== Drawing ====
- (void) drawInDrawable:(id<CAMetalDrawable>)drawable
        inCommandBuffer:(id<MTLCommandBuffer>)command_buffer
{
    [_draw_subroutine encodeSubroutineInBuffer:command_buffer
                                     inTexture:drawable.texture];
}

- (void) predrawOpenInBuffers {
    [_shape_vertices predrawOpen];
}

- (void) predrawCloseInBuffers {
    [_shape_vertices predrawClose];
}

- (void) drawCompletedInBuffers {
    [_shape_vertices drawCompleted];
}

@end
