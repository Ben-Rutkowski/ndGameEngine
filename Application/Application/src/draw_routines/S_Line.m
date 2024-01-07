#import "DrawRoutines.h"

@implementation LineSubroutine
{
    id<MTLRenderPipelineState> _draw_line;
}

- (instancetype)initWithDevice:(nonnull id<MTLDevice>)device
                       library:(nonnull id<MTLLibrary>)library
                   pixelFormat:(MTLPixelFormat)pixel_format
{
    self = [super initWithDevice:device library:library];
    if (self) {
//        --- Pipeline
        [self renderSetVertexFunction:@"Line_Triangalized_vertexShader"
               fragmentFunction:@"Line_Triangalized_fragmentShader"
                        library:library];
        [self renderSetPixelFormat:pixel_format];
        [self renderSetVertexBufferImmutable:Line_TriagVidx_vertices];
        _draw_line = [self compileRenderPipeline];
        
//        --- Render Pass ---
        [self setClearColor:MTLClearColorMake(0.0, 0.5, 0.5, 1.0)];
        [self finalizeRenderPass];
        
//        --- Finalize ---
        [self finializeConfig];
    }
    return self;
}


// ==== Draw ====
- (void) encodeSubroutineInBuffer:(nonnull id<MTLCommandBuffer>)command_buffer
                        inTexture:(nonnull id<MTLTexture>)texture
{
    const float aspect_ratio = 600.0/800.0;
    MTLRenderPassDescriptor* render_descriptor = [self currentRenderPassDescriptor:texture];
    
    @autoreleasepool {
        id<MTLRenderCommandEncoder> render_encoder = [command_buffer renderCommandEncoderWithDescriptor:render_descriptor];
        
        [render_encoder setRenderPipelineState:_draw_line];
        
        [render_encoder setVertexBuffer:[[self buffer:S_Main] drawTap]
                                 offset:0
                                atIndex:Line_TriagVidx_vertices];
        
        [render_encoder setVertexBytes:&aspect_ratio
                                length:sizeof(aspect_ratio)
                               atIndex:Line_TriagVidx_aspect_ratio];
        
        [render_encoder drawPrimitives:MTLPrimitiveTypeTriangle
                           vertexStart:0
                           vertexCount:[[self buffer:S_Main] getVertexCount]];
        
        [render_encoder endEncoding];
    }
}

@end
