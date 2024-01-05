#import "DrawRoutines.h"

@implementation StaticShapeSubroutine
{
    id<MTLRenderPipelineState> _draw_pipeline;
    
    DynamicBuffer* _vertex_buffer;
}

// ==== Configuring ====
- (void) configureWithDrawablePixelFormat:(MTLPixelFormat)pixel_format {
//    ---- Pipeline ---
    [self setVertexFunction:@"StaticShape_vertexShader"
           fragmentFunction:@"StaticShape_fragmentShader"];
    [self setPixelFormat:pixel_format];
    _draw_pipeline = [self compileRenderPipeline];
    
//    --- Render Pass ---
    [self setClearColor:MTLClearColorMake(0.0, 0.5, 0.5, 1.0)];
    [self finalizeRenderPass];
    
//    --- Finalize ---
    [self finializeConfig];
}

- (void) linkBuffer:(nonnull DynamicBuffer*)buffer {
    _vertex_buffer = buffer;
}

// ==== Resources ====
- (void) bindBuffer:(NSUInteger)index {
}

// ==== Draw ====
- (void) encodeSubroutineInBuffer:(nonnull id<MTLCommandBuffer>)command_buffer
                        inTexture:(nonnull id<MTLTexture>)texture 
{
    const float aspect_ratio = (float)600/(float)800;
    
    MTLRenderPassDescriptor* current_pass = [self currentRenderPassDescriptor:texture];
    
    @autoreleasepool {
        id<MTLRenderCommandEncoder> render_encoder = [command_buffer renderCommandEncoderWithDescriptor:current_pass];
        
        [render_encoder setRenderPipelineState:_draw_pipeline];
        
        [render_encoder setVertexBuffer:[_vertex_buffer drawTap]
                                 offset:0
                                atIndex:StaticShape_VertexIndex_vertices];
        
        [render_encoder setVertexBytes:&aspect_ratio
                                length:sizeof(aspect_ratio)
                               atIndex:StaticShape_VertexIndex_aspect_ratio];

        [render_encoder drawPrimitives:MTLPrimitiveTypeTriangle
                           vertexStart:0
                           vertexCount:[_vertex_buffer getVertexCount]];

        [render_encoder endEncoding];
    }
}

@end
