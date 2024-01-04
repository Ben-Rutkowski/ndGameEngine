#ifndef DRAW_ROUTINE_TEMPLATES_H
#define DRAW_ROUTINE_TEMPLATES_H

#import <MetalKit/MetalKit.h>
#import "Buffers.h"
#import "shader_types.h"


// ================ Subroutine Protocol ================
@protocol DrawSubroutineProtocol
// --- Configure ---
- (void) encodeSubroutineInBuffer:(nonnull id<MTLCommandBuffer>)command_buffer
                        inTexture:(nonnull id<MTLTexture>)texture;
- (void) configureWithDrawablePixelFormat:(MTLPixelFormat)pixel_format;

// --- Resources ---
- (void) bindBuffer:(NSUInteger)index;
- (void) linkBuffer:(nonnull DynamicBuffer*)buffer;

// ---------------- ----------------
- (void) linkBufferOLD:(nonnull id<MTLBuffer>)buffer vertexCount:(NSUInteger)count;
@end


// ================ Subroutine Template ================
@interface DrawSubroutineTemplate : NSObject
// --- Configure ---
- (nonnull instancetype) initWithDevice:(nonnull id<MTLDevice>)device
                                library:(nonnull id<MTLLibrary>)library;
// --- Pipeline ---
- (void) setVertexFunction:(nonnull NSString*)vertex_name fragmentFunction:(nonnull NSString*)fragment_name;
- (void) setPixelFormat:(MTLPixelFormat)pixel_format;
- (void) setVertexBufferImmutable:(NSUInteger)index;
- (void) enableIndirectCommandBuffer;
- (nullable id<MTLRenderPipelineState>) compilePipeline;

// --- Render Pass ---
- (void) setClearColor:(MTLClearColor)color;
- (void) finalizeRenderPass;

// --- Indirect Command Buffer ---
- (nullable id<MTLIndirectCommandBuffer>) setUpICBVertexBufferCount:(NSUInteger)vertex_count
                                                fragmentBufferCount:(NSUInteger)fragment_count
                                                         maxCommand:(NSUInteger)max_command_count;

// --- Finalizing ---
- (void) finializeConfig;

// --- Draw ---
- (nonnull MTLRenderPassDescriptor*) currentRenderPassDescriptor:(nonnull id<MTLTexture>)texture;
@end


// ================ Routine Protocol ================
@protocol DrawRoutineProtocol
// --- Configure ---
- (nonnull instancetype) initWithDevice:(nonnull id<MTLDevice>)device
                                library:(nonnull id<MTLLibrary>)library;
- (void) configureWithDrawablePixelFormat:(MTLPixelFormat)pixel_format;

// --- Resources ---
- (void) bindBuffer:(NSUInteger)buffer_index;
- (void) createBufferWithVertexCount:(NSUInteger)count;
- (nullable DynamicBuffer*) getBuffer;

// --- Draw ---
- (void) drawInDrawable:(nonnull id<CAMetalDrawable>)drawable
        inCommandBuffer:(nonnull id<MTLCommandBuffer>)command_buffer;
- (void) beforeDraw;
- (void) drawUntapScheduled;
- (void) drawUntapCompleted;

// ---------------- ----------------
- (nullable id<MTLBuffer>) getBufferOLD;
- (void) OLDdrawInDrawable:(nonnull id<CAMetalDrawable>)drawable
           inCommandBuffer:(nonnull id<MTLCommandBuffer>)command_buffer;
@end


// ================ Routine Template ================
@interface DrawRoutineTemplate : NSObject
// --- Configure ---
- (nonnull instancetype) initWithDevice:(nonnull id<MTLDevice>)device;

// --- Resources ---
- (nonnull DynamicBuffer*) newDynamicBufferWithDataSize:(NSUInteger)data_size
                                            vertexCount:(NSUInteger)vertex_count
                                            storageMode:(MTLResourceOptions)storage_mode;

// ---------------- ----------------
- (nullable id<MTLBuffer>) OLDnewSharedBufferWithLength:(NSUInteger)length;
- (nullable id<MTLBuffer>) OLDnewPrivateBufferWithLength:(NSUInteger)length;

@end


// ================ Null Draw Routine ================
@interface NullDrawRoutine : NSObject<DrawRoutineProtocol>
@end

#endif
