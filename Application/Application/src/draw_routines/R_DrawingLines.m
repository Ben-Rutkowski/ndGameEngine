#import "DrawRoutines.h"

@implementation DrawingLines
{
    
}


// ==== Configure ====
- (instancetype) initWithDevice:(id<MTLDevice>)device
                        library:(id<MTLLibrary>)library
{
    return nil;
}
- (void) configureWithDrawablePixelFormat:(MTLPixelFormat)pixel_format {}
- (void) createBufferWithVertexCount:(NSUInteger)count {}


// ==== Resources ====
- (void) bindBuffer:(NSUInteger)buffer_index {}
- (void) writeBufferClose {}
- (id<MTLBuffer>) writeBufferOpen { return nil; }


// ==== Draw ====
- (void) drawInDrawable:(id<CAMetalDrawable>)drawable
        inCommandBuffer:(id<MTLCommandBuffer>)command_buffer
{}

- (void)predrawOpenInBuffers {}
- (void)predrawCloseInBuffers {}
- (void)drawCompletedInBuffers {}

@end
