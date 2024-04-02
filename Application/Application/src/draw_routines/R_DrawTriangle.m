#import "DrawRoutines.h"
#import "draw_routine_indices.h"

// #define LINEBUFFERCOUNT 3

@implementation DrawTriangle
{
    TriangleSubroutine* _triangle_subroutine;
}


// ==== Configure ====
- (instancetype) initWithDevice:(id<MTLDevice>)device 
                   commandQueue:(id<MTLCommandQueue>)command_queue 
                        library:(id<MTLLibrary>)library 
                    pixelFormat:(MTLPixelFormat)pixel_format 
{
    self = [super initWithDevice:device 
                    commandQueue:command_queue 
          numberOfDynamicBuffers:2
           numberOfStaticBuffers:0];

    if (self) {
        _triangle_subroutine = [[TriangleSubroutine alloc]
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
    [_triangle_subroutine linkBuffer:[self bufferAt:R_Triangle_Vertices] 
                             atIndex:S_Main];
    [_triangle_subroutine linkBuffer:[self bufferAt:R_Triangle_FrameData] 
                             atIndex:S_Aux0];
    [_triangle_subroutine encodeSubroutineInBuffer:command_buffer 
                                         inTexture:drawable.texture];
}

@end
