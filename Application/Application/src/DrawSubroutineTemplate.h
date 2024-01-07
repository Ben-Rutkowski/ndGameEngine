#ifndef DRAW_SUBROUTINE_TEMPLATE_H
#define DRAW_SUBROUTINE_TEMPLATE_H

#import <MetalKit/MetalKit.h>
#import "Buffers.h"
//#import "shader_types.h"

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
- (void) bindBufferMIGHTBEPOINTLESS:(SubroutineEnum)index;
- (void) linkBuffer:(nonnull DynamicBuffer*)buffer
            atIndex:(SubroutineEnum)index;
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

#endif
