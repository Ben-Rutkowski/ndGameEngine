#import "DrawRoutines.h"

#define LINEBUFFERCOUNT 3

@implementation DrawingLines
{
    LineSubroutine* _draw_line_subroutine;
}


// ==== Configure ====
- (nonnull instancetype) initWithDevice:(nonnull id<MTLDevice>)device
                           commandQueue:(nonnull id<MTLCommandQueue>)command_queue
                                library:(nonnull id<MTLLibrary>)library
                            pixelFormat:(MTLPixelFormat)pixel_format
{
    self = [super initWithDevice:device 
                    commandQueue:command_queue
                 numberOfBuffers:R_Debug_BufferCount];
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
    [_draw_line_subroutine linkBuffer:[self bufferAt:R_Debug_Buffer0]
                              atIndex:S_Main];
    [_draw_line_subroutine linkBuffer:[self bufferAt:R_Debug_Buffer1]
                              atIndex:S_Aux0];
    [_draw_line_subroutine encodeSubroutineInBuffer:command_buffer
                                          inTexture:drawable.texture];
}

@end
