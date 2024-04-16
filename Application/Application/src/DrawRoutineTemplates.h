#ifndef DRAW_ROUTINE_TEMPLATES_H
#define DRAW_ROUTINE_TEMPLATES_H

#import <MetalKit/MetalKit.h>
#include <objc/NSObjCRuntime.h>
#import "Buffers.h"
// #import "shader_types.h"


// ================ Routine Template ================
@interface DrawRoutineTemplate : NSObject
- (nonnull instancetype) initWithDevice:(nonnull id<MTLDevice>)device
                           commandQueue:(nonnull id<MTLCommandQueue>)command_queue
                 numberOfDynamicBuffers:(NSUInteger)dynamic_buffer_count
                  numberOfStaticBuffers:(NSUInteger)static_buffer_count;

// --- Resources ---
- (void) createBufferWithVertexSize:(NSUInteger)vertex_size
                        vertexCount:(NSUInteger)vertex_count
                         bufferType:(NSUInteger)buffer_type;

- (void) bindBuffer:(NSUInteger)index;
- (nullable id<BufferProtocol>) bufferAt:(NSUInteger)index;
- (nullable id<MTLBuffer>) writeBufferOpen;
- (void) writeBufferClose;
- (void) writeBufferCloseWithRange:(NSRange)range;

// --- Draw ---
- (void) predrawOpenInBuffers;
- (void) predrawCloseInBuffers;
- (void) drawCompletedInBuffers;

// --- Debug ---
- (void) debugBuffer:(NSUInteger)vertex_count;

@end

#endif
