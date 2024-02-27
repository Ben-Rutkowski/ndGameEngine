#import "DrawRoutines.h"
#import "shader_types/shader_types.h"

@implementation LineSubroutine
{
    id<MTLRenderPipelineState>  _draw_line;
    id<MTLComputePipelineState> _trianglize;
}

- (instancetype)initWithDevice:(nonnull id<MTLDevice>)device
                       library:(nonnull id<MTLLibrary>)library
                   pixelFormat:(MTLPixelFormat)pixel_format
{
    self = [super initWithDevice:device library:library];
    if (self) {
//        --- Draw Pipeline ---
        [self renderSetVertexFunction:@"Line_Triangalized_vertexShader"
                     fragmentFunction:@"Line_Triangalized_fragmentShader"
                              library:library];
        [self renderSetPixelFormat:pixel_format];
        [self renderSetVertexBufferImmutable:Line_TriagVidx_vertices];
        _draw_line = [self compileRenderPipeline];
        
//        --- Compute Vertcies Pipeline ---
        _trianglize = [self computePipelineWithFunctionName:@"Line_Internal_computeShader"
                                                    library:library];
        
//        --- Finalize ---
        [self setClearColor:MTLClearColorMake(0.0, 0.5, 0.5, 1.0)];
        [self finalizeRenderPass];
        [self finializeConfig];
    }
    return self;
}


// ==== Draw ====
- (void) encodeSubroutineInBuffer:(nonnull id<MTLCommandBuffer>)command_buffer
                        inTexture:(nonnull id<MTLTexture>)texture
{
    const float aspect_ratio = 600.0/800.0;
    
    NSUInteger thread_size     = [[self buffer:S_Main] getVertexCount]/2;
    MTLSize    mtl_thread_size = MTLSizeMake(thread_size, 1, 1);
    
    NSUInteger thread_group_size = _trianglize.maxTotalThreadsPerThreadgroup;
    if (thread_group_size > thread_size) {
        thread_group_size = thread_size;
    }
    MTLSize mtl_thread_group_size = MTLSizeMake(thread_group_size, 1, 1);
    
    MTLRenderPassDescriptor* render_descriptor = [self currentRenderPassDescriptor:texture];
    @autoreleasepool {
//        --- Trianglize ---
        id<MTLComputeCommandEncoder> compute_encoder = [command_buffer computeCommandEncoder];
        [compute_encoder setComputePipelineState:_trianglize];
        [compute_encoder setBuffer:[[self buffer:S_Main] drawTap]
                            offset:0
                           atIndex:Line_PtVidx_vertices];
        [compute_encoder setBuffer:[[self buffer:S_Aux0] drawRelay]
                            offset:0
                           atIndex:Line_PtVidx_triag_cluster];
        [compute_encoder dispatchThreads:mtl_thread_size
                   threadsPerThreadgroup:mtl_thread_group_size];
        [compute_encoder endEncoding];
        
//        --- Draw Triangle ---
        id<MTLRenderCommandEncoder> render_encoder = [command_buffer renderCommandEncoderWithDescriptor:render_descriptor];
        [render_encoder setRenderPipelineState:_draw_line];
        [render_encoder setVertexBuffer:[[self buffer:S_Aux0] drawRelay]
                                 offset:0
                                atIndex:Line_TriagVidx_vertices];
        [render_encoder setVertexBytes:&aspect_ratio
                                length:sizeof(aspect_ratio)
                               atIndex:Line_TriagVidx_aspect_ratio];
        [render_encoder drawPrimitives:MTLPrimitiveTypeTriangle
                           vertexStart:0
                           vertexCount:[[self buffer:S_Aux0] getVertexCount]];
        [render_encoder endEncoding];
    }
}

@end
