#import "DrawRoutines.h"
#include "draw_routine_indices.h"

#define LINEBUFFERCOUNT 3

@implementation DrawingLines
{
    ThickLineSubroutine* _draw_line_subroutine;
}


// ==== Configure ====
- (nonnull instancetype) initWithDevice:(nonnull id<MTLDevice>)device
                           commandQueue:(nonnull id<MTLCommandQueue>)command_queue
                                library:(nonnull id<MTLLibrary>)library
                            pixelFormat:(MTLPixelFormat)pixel_format
{
    self = [super initWithDevice:device 
                    commandQueue:command_queue 
          numberOfDynamicBuffers:R_Debug_BufferCount 
           numberOfStaticBuffers:0];

    if (self) {
        _draw_line_subroutine = [[ThickLineSubroutine alloc]
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
