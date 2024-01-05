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

// --- Size ---
- (NSUInteger) getVertexCount;
- (void) expandToDataSize:(NSUInteger)size
           andVertexCount:(NSUInteger)vertex_count
               withDevice:(nonnull id<MTLDevice>)device
           inCommandQueue:(nonnull id<MTLCommandQueue>)command_queue;

// --- Write ---
- (nonnull id<MTLBuffer>) writeOpen;
- (void) writeCloseInCommandBuffer:(nonnull id<MTLCommandBuffer>)command_buffer;

// --- Draw ---
- (nonnull id<MTLBuffer>) drawTap;
- (void) beginPredrawStage;
- (void) endPredrawStage;
- (void) endDrawStage;

// --- Debug ---
- (void) debug;

@end

#endif
