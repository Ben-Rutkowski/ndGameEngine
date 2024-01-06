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


- (nonnull id<MTLCommandBuffer>) getBlitCommandBuffer;
- (void) bindBuffer:(NSUInteger)index;
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

@end


// ================ Subroutine Enums ================
typedef enum SubroutineEnum {
    S_Main = 0,  S_Aux0 = 1,  S_Aux1 = 2,
    S_Aux2 = 3,  S_Aux3 = 4
} SubroutineEnum;


// ================ Subroutine Protocol ================
@protocol DrawSubroutineProtocol
- (nonnull instancetype) initWithDevice:(nonnull id<MTLDevice>)device
                                library:(nonnull id<MTLLibrary>)library
                            pixelFormat:(MTLPixelFormat)pixel_format;

- (void) encodeSubroutineInBuffer:(nonnull id<MTLCommandBuffer>)command_buffer
                        inTexture:(nonnull id<MTLTexture>)texture;

// --- Depricated ---
- (void) configureWithDrawablePixelFormatOLD:(MTLPixelFormat)pixel_format;

@end


// ================ Subroutine Template ================
@interface DrawSubroutineTemplate : NSObject
- (nonnull instancetype) initWithDevice:(nonnull id<MTLDevice>)device
                                library:(nonnull id<MTLLibrary>)library;

- (nonnull id<MTLBuffer>) newAuxBufferOfSize:(NSUInteger)data_size;

// --- Resources ---
- (void) bindBuffer:(SubroutineEnum)index;
- (void) linkBuffer:(nonnull DynamicBuffer*)buffer;
- (nonnull DynamicBuffer*) buffer:(SubroutineEnum)index;

// --- Render Pipeline ---
- (void) renderSetVertexFunction:(nonnull NSString*)vertex_name
          fragmentFunction:(nonnull NSString*)fragment_name
                   library:(nonnull id<MTLLibrary>)library;
- (void) renderSetPixelFormat:(MTLPixelFormat)pixel_format;
- (void) renderSetVertexBufferImmutable:(NSUInteger)index;
- (void) renderEnableIndirectCommandBuffer;
- (nullable id<MTLRenderPipelineState>) compileRenderPipeline;

// --- Compute Pipeline ---
- (nullable id<MTLComputePipelineState>) computePipelineWithFunctionName:(nonnull NSString*)name
                                                                 library:(nonnull id<MTLLibrary>)library;

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


// --- Depricated ---
- (nullable id<MTLComputePipelineState>) computePipelineWithFunctionNameOLD:(nonnull NSString*)name;

- (void) setVertexFunctionOLD:(nonnull NSString*)vertex_name
          fragmentFunction:(nonnull NSString*)fragment_name;
@end


// ================ Null Draw Routine ================
@interface NullDrawRoutine : NSObject<DrawRoutineProtocol>
@end

#endif
