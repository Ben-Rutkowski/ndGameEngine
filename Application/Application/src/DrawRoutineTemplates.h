#ifndef DRAW_ROUTINE_TEMPLATES_H
#define DRAW_ROUTINE_TEMPLATES_H

#import <MetalKit/MetalKit.h>
#import "Buffers.h"
// #import "shader_types.h"


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
- (nullable id<BufferProtocol>) bufferAt:(NSUInteger)index;
- (nullable id<MTLBuffer>) writeBufferOpen;
- (void) writeBufferClose;

// --- Draw ---
- (void) predrawOpenInBuffers;
- (void) predrawCloseInBuffers;
- (void) drawCompletedInBuffers;

// --- Debug ---
- (void) debugBuffer:(NSUInteger)vertex_count;

@end

#endif
