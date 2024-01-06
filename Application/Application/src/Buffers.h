#ifndef BUFFERS_H
#define BUFFERS_H

#import <Metal/Metal.h>

/*
    Resizeable Buffers can be resized and edited.
    But these opperations are slow, so they are not meant to
    be resized or edited every frame.
*/
@interface DynamicBuffer : NSObject
// --- Configure ---
- (nonnull instancetype) initWithDevice:(nonnull id<MTLDevice>)device
                               vertexSize:(NSUInteger)vertex_size
                            vertexCount:(NSUInteger)vertex_count
                         andStorageMode:(MTLResourceOptions)storage_mode;

- (NSUInteger) getVertexCount;

// --- Draw ---
- (nonnull id<MTLBuffer>) drawTap;
- (void) predrawOpen;
- (void) predrawClose;
- (void) drawCompleted;

// --- Write ---
- (nonnull id<MTLBuffer>) writeOpen;
- (void) writeCloseInBlitCommandBuffer:(nonnull id<MTLCommandBuffer>)command_buffer;

// --- Size ---
//- (void) expandToDataSize:(NSUInteger)size
//           andVertexCount:(NSUInteger)vertex_count
//               withDevice:(nonnull id<MTLDevice>)device
//           inCommandQueue:(nonnull id<MTLCommandQueue>)command_queue;

// --- Debug ---
- (void) debug:(NSUInteger)num_vertices;

@end

#endif
