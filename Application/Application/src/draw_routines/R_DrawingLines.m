#import "DrawRoutines.h"

#define LINEBUFFERCOUNT 1

@implementation DrawingLines
{
    LineSubroutine* _draw_line_subroutine;
    
//    --- Depricated ---
//    DynamicBuffer*  _buffer;
}


// ==== Configure ====
- (nonnull instancetype)initWithDevice:(nonnull id<MTLDevice>)device
                               library:(nonnull id<MTLLibrary>)library
                           pixelFormat:(MTLPixelFormat)pixel_format
{
    self = [super initWithDevice:device 
                 numberOfBuffers:LINEBUFFERCOUNT];
    if (self) {
        _draw_line_subroutine = [[LineSubroutine alloc]
                                 initWithDevice:device
                                        library:library
                                    pixelFormat:pixel_format];
    }
    return self;
}


// ==== Draw ====
- (void) drawInDrawable:(id<CAMetalDrawable>)drawable
        inCommandBuffer:(id<MTLCommandBuffer>)command_buffer
{
    [_draw_line_subroutine linkBuffer:[self bufferAt:Line_B_Vertices]
                              atIndex:S_Main];
    
    [_draw_line_subroutine encodeSubroutineInBuffer:command_buffer
                                          inTexture:drawable.texture];
}


// ==== Depricated ====
- (void) bindBufferOLD:(NSUInteger)buffer_index {}

- (id<MTLBuffer>) writeBufferOpenOLD {
//    return [_buffer writeOpen];
    return nil;
}

- (void) writeBufferCloseOLD {
    @autoreleasepool {
//        id<MTLCommandBuffer> blit_command_buffer = [self getBlitCommandBuffer];
//        [_buffer writeCloseInBlitCommandBuffer:blit_command_buffer];
//        [blit_command_buffer commit];
    }
}

- (void) createBufferWithVertexCountOLD:(NSUInteger)count {
//    _buffer = [self newDynamicBufferWithVertexSizeOLD:sizeof(Line_TriagVtype)
//                                       vertexCount:count
//                                       storageMode:MTLResourceStorageModeShared];
//    [_draw_line_subroutine bindBuffer:S_Main];
//    [_draw_line_subroutine linkBuffer:_buffer];
}

- (instancetype) initWithDeviceOLD:(id<MTLDevice>)device
                        library:(id<MTLLibrary>)library
{
    self = [super initWithDevice:device
                 numberOfBuffers:LINEBUFFERCOUNT];
    if (self) {
        _draw_line_subroutine = [[LineSubroutine alloc] initWithDevice:device
                                                               library:library];
    }
    return self;
}

- (void)predrawOpenInBuffersOLD {
//    [_buffer predrawOpen];
}

- (void)predrawCloseInBuffersOLD {
//    [_buffer predrawClose];
}

- (void)drawCompletedInBuffersOLD {
//    [_buffer drawCompleted];
}

- (void) configureWithDrawablePixelFormatOLD:(MTLPixelFormat)pixel_format {
    [_draw_line_subroutine configureWithDrawablePixelFormatOLD:pixel_format];
}


@end
