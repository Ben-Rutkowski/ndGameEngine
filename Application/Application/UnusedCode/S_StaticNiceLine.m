#import "DrawRoutines.h"

@implementation StaticNiceLineSubroutine
{
    id<MTLRenderPipelineState> _draw_line_pipeline;
    
    id<MTLBuffer> _vertex_data_buffer;
    NSUInteger    _vertex_data_count;
}

- (void)linkVertexDataBuffer:(nonnull id<MTLBuffer>)vertex_data 
                 vertexCount:(NSUInteger)count{
    _vertex_data_buffer = vertex_data;
    _vertex_data_count  = count;
}

- (void)configureWithDrawablePixelFormat:(MTLPixelFormat)pixel_format {
//    --- Pipeline ---
    [self setVertexFunction:@"NiceLine_vertexShader" fragmentFunction:@"NiceLine_fragmentShader"];
    [self setPixelFormat:pixel_format];
    [self setVertexBufferImmutable:NiceLine_VertexIndex_vertices];
    _draw_line_pipeline = [self compilePipeline];
    
//    --- Render Pass ---
    [self setClearColor:MTLClearColorMake(0.0, 0.5, 0.5, 1.0)];
    [self finalizeRenderPass];
    
    [self finializeConfig];
}

- (void)encodeSubroutineInBuffer:(nonnull id<MTLCommandBuffer>)command_buffer 
                       inTexture:(nonnull id<MTLTexture>)texture
{
    @autoreleasepool {
        MTLRenderPassDescriptor*    current_pass    = [self currentRenderPassDescriptor:texture];
        id<MTLRenderCommandEncoder> command_encoder = [command_buffer renderCommandEncoderWithDescriptor:current_pass];
        
        [command_encoder setRenderPipelineState:_draw_line_pipeline];
        
        [command_encoder setVertexBuffer:_vertex_data_buffer
                                  offset:0
                                 atIndex:NiceLine_VertexIndex_vertices];
        
        const float aspect_ratio = 800.0/600.0;
        
        [command_encoder setVertexBytes:&aspect_ratio
                                 length:sizeof(aspect_ratio)
                                atIndex:NiceLine_VertexIndex_aspect_ratio];
        
        [command_encoder drawPrimitives:MTLPrimitiveTypeTriangle
                            vertexStart:0
                            vertexCount:_vertex_data_count];
        
        [command_encoder endEncoding];
    }
}

@end
