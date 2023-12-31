#ifndef DRAW_ROUTINES_H
#define DRAW_ROUTINES_H

#import <MetalKit/MetalKit.h>
#import "shader_types.h"

/*
Subroutine:
    Init
    - Pipeline State
    - Render Pass Descriptor
    - Indirect Command Buffer
 
    Drawing
    - Needs drawable
    - Has a semaphore wait function for compute shared data
    - Very short endcoding phase
    - Commit to GPU
*/

@protocol DrawSubroutineProtocol
- (void) encodeSubroutineInBuffer:(nonnull id<MTLCommandBuffer>)command_buffer
                        inTexture:(nonnull id<MTLTexture>)texture;
@end

@interface DrawSubroutineTemplate : NSObject
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
- (void) finializeInit;

// --- Drawing ---
- (nonnull MTLRenderPassDescriptor*) currentRenderPassDescriptor:(nonnull id<MTLTexture>)texture;

@end

/*
Routine:
    Init
    - Initialize subroutines
    - Initialize semaphore
 
     Resources
     - Private Buffers
     - Shared Buffers
     - (Textures)
     - Update shared data
     - Reload resources
    
    Drawing
    - Waiting to reload shared data
    - Draw in drawable with command buffer
 
*/

@protocol DrawRoutineProtocol
- (void) reloadSharedData;
- (void) drawInDrawable:(nonnull id<CAMetalDrawable>)drawable inCommandBuffer:(nonnull id<MTLCommandBuffer>)command_buffer;
@end

// ================ SUBROUTINES ================

/*
Static Shape:
    - Private Buffer: VertexData
*/

@interface StaticShapeSubroutine : DrawSubroutineTemplate<DrawSubroutineProtocol>
- (void) linkVertexDataBuffer:(nonnull id<MTLBuffer>)vertex_data;
- (void) configureWithDrawablePixelFormat:(MTLPixelFormat)pixel_format;
@end


// ================ ROUTINES ================
@interface StaticShapeRoutine : NSObject<DrawRoutineProtocol>
- (nonnull instancetype) initWithDevice:(nonnull id<MTLDevice>)device
                                library:(nonnull id<MTLLibrary>)library;

- (void) configureWithDrawablePixelFormat:(MTLPixelFormat)pixel_format;

// --- Buffers ---
- (nonnull StaticShape_VertexType*) createVertexBufferWithVertexNumber:(NSUInteger)number;
@end

#endif
