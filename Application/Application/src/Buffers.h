#ifndef BUFFERS_H
#define BUFFERS_H

#import <Metal/Metal.h>

@interface ResizableBuffer : NSObject
// --- Configure ---
- (nonnull instancetype) initWithDevice:(nonnull id<MTLDevice>)device
                               dataSize:(NSUInteger)data_size
                            vertexCount:(NSUInteger)vertex_count
                         andStorageMode:(MTLResourceOptions)storage_mode;

// --- Size ---
- (NSUInteger) getVertexCount;
- (void) expandToDataSize:(NSUInteger)size
           andVertexCount:(NSUInteger)vertex_count
               withDevice:(nonnull id<MTLDevice>)device
           inCommandQueue:(nonnull id<MTLCommandQueue>)command_queue;

// --- Access ---
- (nonnull id<MTLBuffer>) drawTap;
- (void) beginPredrawStage;
- (void) endPredrawStage;
- (void) beginDrawStage;
- (void) endDrawStage;
//- (void) endDrawStageWithBufferIndex:(NSUInteger)index;

- (nonnull id<MTLBuffer>) editTap;
- (void) editUntap;

// --- Debug ---
- (void) debug;

@end

#endif
