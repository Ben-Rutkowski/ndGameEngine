/*
    Templates hold the device and library
    Derived class must have no knowledge of the device
*/

#ifndef DRAW_ROUTINE_TEMPLATES_H
#define DRAW_ROUTINE_TEMPLATES_H

#import <MetalKit/MetalKit.h>
#import "shader_types.h"


// ==== Protocol ====
@protocol DrawSubroutineProtocol
- (void) encodeSubroutineInBuffer:(nonnull id<MTLCommandBuffer>)command_buffer
                        inTexture:(nonnull id<MTLTexture>)texture;
- (void) configureWithDrawablePixelFormat:(MTLPixelFormat)pixel_format;
@end


// ==== Template ====
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
- (void) finializeConfig;

// --- Drawing ---
- (nonnull MTLRenderPassDescriptor*) currentRenderPassDescriptor:(nonnull id<MTLTexture>)texture;

@end


// ==== Protocol ====
@protocol DrawRoutineProtocol
- (nonnull instancetype) initWithDevice:(nonnull id<MTLDevice>)device
                                library:(nonnull id<MTLLibrary>)library;
- (void) configureWithDrawablePixelFormat:(MTLPixelFormat)pixel_format;
- (void) drawInDrawable:(nonnull id<CAMetalDrawable>)drawable inCommandBuffer:(nonnull id<MTLCommandBuffer>)command_buffer;

//- (void) reloadSharedData;
@end


// ==== Template ====
@interface DrawRoutineTemplate : NSObject
- (nonnull instancetype) initWithDevice:(nonnull id<MTLDevice>)device;
- (nonnull id<MTLBuffer>) newSharedBufferWithLength:(NSUInteger)length;
- (nonnull id<MTLBuffer>) newPrivateBufferWithLength:(NSUInteger)length;
@end

#endif
