#import "DrawRoutines.h"
#import "draw_routine_indices.h"

@implementation DrawingLines
{
    ThickLineSubroutine* _draw_line_subroutine;
}


// ==== Configure ====
- (instancetype) initWithDevice:(nonnull id<MTLDevice>)device
                   commandQueue:(nonnull id<MTLCommandQueue>)command_queue
                        library:(nonnull id<MTLLibrary>)library
                    pixelFormat:(MTLPixelFormat)pixel_format
{
    self = [super initWithDevice:device 
                    commandQueue:command_queue 
          numberOfDynamicBuffers:R_Debug_DynamicBufferCount 
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
    [_draw_line_subroutine linkBuffer:[self bufferAt:R_Debug_DynamicBuffer0] 
                              atIndex:S_Main];
    [_draw_line_subroutine linkBuffer:[self bufferAt:R_Debug_DynamicBuffer1] 
                              atIndex:S_Aux0];
    [_draw_line_subroutine linkBuffer:[self bufferAt:R_Debug_DynamicBuffer2] 
                              atIndex:S_Aux1];
    [_draw_line_subroutine encodeSubroutineInBuffer:command_buffer 
                                          inTexture:drawable.texture];
}

@end
