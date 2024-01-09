#ifndef BUFFERS_H
#define BUFFERS_H

#import <Metal/Metal.h>

/*
    Resizeable Buffers can be resized and edited.
    But these opperations are slow, so they are not meant to
    be resized or edited every frame.
*/
@protocol BufferProtocol
- (NSUInteger) getVertexCount;

// --- Draw ---
- (nullable id<MTLBuffer>) drawTap;
- (nullable id<MTLBuffer>) drawRelay;
- (void) predrawOpen;
- (void) predrawClose;
- (void) drawCompleted;

// --- Write ---
- (nullable id<MTLBuffer>) writeOpen;
- (void) writeClose;

// --- Debug ---
- (void) debug:(NSUInteger)num_vertices;
@end


@interface DynamicBuffer : NSObject<BufferProtocol>
// --- Configure ---
- (nonnull instancetype) initWithDevice:(nonnull id<MTLDevice>)device
                       blitCommandQueue:(nonnull id<MTLCommandQueue>)command_queue
                             vertexSize:(NSUInteger)vertex_size
                            vertexCount:(NSUInteger)vertex_count
                         andStorageMode:(MTLResourceOptions)storage_mode;

// --- Debug ---
- (void) debug:(NSUInteger)num_vertices;

@end


// ================ Null Buffer ================
@interface NullBuffer : NSObject<BufferProtocol>
@end

#endif
