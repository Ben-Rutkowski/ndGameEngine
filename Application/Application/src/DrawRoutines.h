#ifndef DRAW_ROUTINES_H
#define DRAW_ROUTINES_H

#import <MetalKit/MetalKit.h>

//@protocol DrawRoutineProtocolOLD
//- (nonnull instancetype) initWithMTKView:(nonnull MTKView*)mtk_view MTLLibrary:(nonnull id<MTLLibrary>)library;
//- (void) commitWithMTKView:(nonnull MTKView*)mtk_view CommandQueue:(nonnull id<MTLCommandQueue>)command_queue;
//@end

@protocol DrawRoutineProtocol
- (nonnull instancetype) initWithDevice:(nonnull id<MTLDevice>)device MTLLibrary:(nonnull id<MTLLibrary>)library drawablePixelFormat:(MTLPixelFormat)drawable_pixel_format;
- (void) commitIntoMetalLayer:(nonnull CAMetalLayer*) metal_layer commandQueue:(nonnull id<MTLCommandQueue>)command_queue;
@end

// ================ Draw Routines ================
//@interface BasicTriangleDrawRoutine : NSObject<DrawRoutineProtocolOLD>
//@end
//
//@interface ParallelTriangleRotateDrawRoutine : NSObject<DrawRoutineProtocolOLD>
//- (void) updateViewPortWithSize:(CGSize)size;
//@end

@interface BasicTriangleCustomViewDrawRoutine : NSObject<DrawRoutineProtocol>
@end

#endif
