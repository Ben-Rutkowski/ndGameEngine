#define __INTERNAL__
#import "DrawRoutines.h"
#import "shader_types/uniform_types.h"
#import "shader_types/S_thick_line_types.h"

@implementation ThickLineSubroutine
{
    id<MTLRenderPipelineState>  _draw_trianglized_line;
    id<MTLComputePipelineState> _compute_trianglized_line;
}

- (instancetype) initWithDevice:(nonnull id<MTLDevice>)device
                        library:(nonnull id<MTLLibrary>)library
                    pixelFormat:(MTLPixelFormat)pixel_format
{
    self = [super initWithDevice:device library:library];
    if (self) {
//        --- Draw Pipeline ---
        [self renderSetVertexFunction:@"ThickLine_vertexShaderOLD"
                     fragmentFunction:@"ThickLine_fragmentShaderOLD"
                              library:library];
        [self renderSetPixelFormat:pixel_format];
        [self renderSetVertexBufferImmutable:ThickLine_vertices_I];
        [self renderSetVertexBufferImmutable:ThickLine_frame_data_I];
        _draw_trianglized_line = [self compileRenderPipeline];

//        --- Compute Pipeline ---
        _compute_trianglized_line = [self computePipelineWithFunctionName:@"INT_ThickLine_computeShader"
                                                                  library:library];

//        --- Finalize ---
        [self setClearColor:MTLClearColorMake(0.5, 0.9, 0.5, 1.0)];
        [self finalizeRenderPass];
        [self finializeConfig];
    }
    return self;
}

- (void) encodeSubroutineInBuffer:(nonnull id<MTLCommandBuffer>)command_buffer
                        inTexture:(nonnull id<MTLTexture>)texture
{
    // NSUInteger thread_count = [[self buffer:S_Main] getVertexCount]/2;
    // MTLSize    mtl_thread_count = MTLSizeMake(thread_count, 1, 1);

    // NSUInteger thread_group_size = _compute_trianglized_line.maxTotalThreadsPerThreadgroup;
    // if (thread_group_size > thread_count) {
    //     thread_group_size = thread_count;
    // }
    // MTLSize mtl_thread_group_size = MTLSizeMake(thread_group_size, 1, 1);

    // MTLRenderPassDescriptor* current_render_pass = [self currentRenderPassDescriptor:texture];
    // @autoreleasepool {
    //     // --- Compute Trianlization ---
    //     id<MTLComputeCommandEncoder> compute_encoder = [command_buffer computeCommandEncoder];
    //     [compute_encoder setComputePipelineState:_compute_trianglized_line];
    //     [compute_encoder setBuffer:[[self buffer:S_Main] drawTap]
    //                         offset:0 
    //                        atIndex:INT_ThickLine_vertices_I];
    //     [compute_encoder setBuffer:[[self buffer:S_Aux0] drawRelay]
    //                         offset:0 
    //                        atIndex:INT_ThickLine_tri_cluster_I];
    //     [compute_encoder setBuffer:[[self buffer:S_Aux1] drawTap]
    //                         offset:0 
    //                        atIndex:INT_ThickLine_frame_data_I];
    //     [compute_encoder dispatchThreads:mtl_thread_count 
    //                threadsPerThreadgroup:mtl_thread_group_size];
    //     [compute_encoder endEncoding];

    //     // --- Draw Triangles ---
    //     id<MTLRenderCommandEncoder> render_encoder = [command_buffer renderCommandEncoderWithDescriptor:current_render_pass];
    //     [render_encoder setRenderPipelineState:_draw_trianglized_line];
    //     [render_encoder setVertexBuffer:[[self buffer:S_Aux0] drawRelay]
    //                              offset:0 
    //                             atIndex:ThickLine_vertices_I];
    //     [render_encoder setVertexBuffer:[[self buffer:S_Aux1] drawTap]
    //                              offset:0
    //                             atIndex:ThickLine_frame_data_I];
    //     [render_encoder drawPrimitives:MTLPrimitiveTypeTriangle
    //                        vertexStart:0 
    //                        vertexCount:[[self buffer:S_Aux0] getVertexCount]];
    //     [render_encoder endEncoding];
    // }
}

@end
