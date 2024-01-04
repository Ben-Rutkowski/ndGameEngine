#ifndef BUFFERS_H
#define BUFFERS_H

#import <Metal/Metal.h>

@interface DynamicBuffer : NSObject

- (nonnull instancetype) initWithDevice:(nonnull id<MTLDevice>)device
                               dataSize:(NSUInteger)data_size
                            vertexCount:(NSUInteger)vertex_count
                         andStorageMode:(MTLResourceOptions)storage_mode;

- (void)expandToDataSize:(NSUInteger)size
          andVertexCount:(NSUInteger)vertex_count
              withDevice:(nonnull id<MTLDevice>)device
          inCommandQueue:(nonnull id<MTLCommandQueue>)command_queue;

- (NSUInteger) getVertexCount;

- (nonnull id<MTLBuffer>) drawTap;
- (void) beforeDraw;
- (void) drawUntapScheduled;
- (void) drawUntapCompleted;

- (nonnull id<MTLBuffer>) editTap;
- (void) editUntap;

- (void) debug;

@end

#endif
