#ifndef DRAW_ROUTINE_TEMPLATES_H
#define DRAW_ROUTINE_TEMPLATES_H

#import <MetalKit/MetalKit.h>
#import "Buffers.h"
#import "shader_types.h"


// ================ Routine Protocol ================
@protocol DrawRoutineProtocol
- (nonnull instancetype) initWithDevice:(nonnull id<MTLDevice>)device
                           commandQueue:(nonnull id<MTLCommandQueue>)command_queue
                                library:(nonnull id<MTLLibrary>)library
                            pixelFormat:(MTLPixelFormat)pixel_format;

// --- Draw ---
- (void) drawInDrawable:(nonnull id<CAMetalDrawable>)drawable
        inCommandBuffer:(nonnull id<MTLCommandBuffer>)command_buffer;

@end


// ================ Routine Template ================
@interface DrawRoutineTemplate : NSObject
- (nonnull instancetype) initWithDevice:(nonnull id<MTLDevice>)device
                           commandQueue:(nonnull id<MTLCommandQueue>)command_queue
                        numberOfBuffers:(NSUInteger)buffer_count;

// --- Resources ---
- (void) createBufferWithVertexSize:(NSUInteger)vertex_size
                        vertexCount:(NSUInteger)vertex_count
                        storageMode:(MTLResourceOptions)storage_mode;


- (void) bindBuffer:(NSUInteger)index;
- (nullable DynamicBuffer*) bufferAt:(NSUInteger)index;
- (nullable id<MTLBuffer>) writeBufferOpen;
- (void) writeBufferClose;

// --- Draw ---
- (void) predrawOpenInBuffers;
- (void) predrawCloseInBuffers;
- (void) drawCompletedInBuffers;

// --- Debug ---
- (void) debugBuffer:(NSUInteger)vertex_count;

@end


// ================ Null Draw Routine ================
@interface NullDrawRoutine : DrawRoutineTemplate<DrawRoutineProtocol>
@end

#endif
