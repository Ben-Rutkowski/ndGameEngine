#ifndef DRAW_ROUTINES_H
#define DRAW_ROUTINES_H

#import <MetalKit/MetalKit.h>

@protocol DrawRoutineProtocol
- (nonnull instancetype) initWithMTKView:(nonnull MTKView*)mtk_view MTLLibrary:(nonnull id<MTLLibrary>)library;
- (void) commitWithMTKView:(nonnull MTKView*)mtk_view CommandQueue:(nonnull id<MTLCommandQueue>)command_queue;
@end

// ================ Draw Routines ================
@interface BasicTriangleDrawRoutine : NSObject<DrawRoutineProtocol>
@end

#endif
