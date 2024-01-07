#ifndef DRAW_ROUTINE_TEMPLATES_H
#define DRAW_ROUTINE_TEMPLATES_H

#import <MetalKit/MetalKit.h>
#import "Buffers.h"
#import "shader_types.h"


// ================ Routine Protocol ================
@protocol DrawRoutineProtocol
- (nonnull instancetype) initWithDevice:(nonnull id<MTLDevice>)device
                                library:(nonnull id<MTLLibrary>)library
                            pixelFormat:(MTLPixelFormat)pixel_format;

// --- Draw ---
- (void) drawInDrawable:(nonnull id<CAMetalDrawable>)drawable
        inCommandBuffer:(nonnull id<MTLCommandBuffer>)command_buffer;


// --- Depricated ---
- (nonnull instancetype) initWithDeviceOLD:(nonnull id<MTLDevice>)device
                                   library:(nonnull id<MTLLibrary>)library;
- (void) createBufferWithVertexCountOLD:(NSUInteger)count;
- (void) configureWithDrawablePixelFormatOLD:(MTLPixelFormat)pixel_format;

- (void) predrawOpenInBuffersOLD;
- (void) predrawCloseInBuffersOLD;
- (void) drawCompletedInBuffersOLD;

- (void) bindBufferOLD:(NSUInteger)buffer_index;
- (nullable id<MTLBuffer>) writeBufferOpenOLD;
- (void) writeBufferCloseOLD;

@end


// ================ Routine Template ================
@interface DrawRoutineTemplate : NSObject
- (nonnull instancetype) initWithDevice:(nonnull id<MTLDevice>)device
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

// --- Depricated ---
- (nonnull DynamicBuffer*) newDynamicBufferWithVertexSizeOLD:(NSUInteger)vertex_size
                                              vertexCount:(NSUInteger)vertex_count
                                              storageMode:(MTLResourceOptions)storage_mode;
//- (nonnull id<MTLCommandBuffer>) getBlitCommandBuffer;

@end


// ================ Null Draw Routine ================
@interface NullDrawRoutine : NSObject<DrawRoutineProtocol>
@end

#endif
