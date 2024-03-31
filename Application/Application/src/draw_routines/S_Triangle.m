#include "DrawSubroutineTemplate.h"
#define __INTERNAL__
#import "DrawRoutines.h"
#import "shader_types/S_triangle_types.h"

@implementation TriangleSubroutine
{
    id<MTLRenderPipelineState> _draw_triangle;
}

- (instancetype) initWithDevice:(id<MTLDevice>)device 
                        library:(id<MTLLibrary>)library 
                    pixelFormat:(MTLPixelFormat)pixel_format 
{
    self = [super initWithDevice:device library:library];
    if (self) {
//        --- Draw Pipeline ---
        [self renderSetVertexFunction:@"Triangle_vertexShader" 
                     fragmentFunction:@"Triangle_fragmentShader"  
                              library:library];
        [self renderSetPixelFormat:pixel_format];
        [self renderSetVertexBufferImmutable:tri_vertices_I];
        [self renderSetVertexBufferImmutable:tri_pers_mat_I];
        _draw_triangle = [self compileRenderPipeline];

//        --- Finalize ---
        [self setClearColor:MTLClearColorMake(0.0, 0.9, 0.5, 1.0)];
        [self finalizeRenderPass];
        [self finializeConfig];
    }
    return self;
}

- (void) encodeSubroutineInBuffer:(id<MTLCommandBuffer>)command_buffer 
                        inTexture:(id<MTLTexture>)texture 
{
    MTLRenderPassDescriptor* current_render_pass = [self currentRenderPassDescriptor:texture];
    @autoreleasepool {
        id<MTLRenderCommandEncoder> render_encoder = [command_buffer renderCommandEncoderWithDescriptor:current_render_pass];
        [render_encoder setRenderPipelineState:_draw_triangle];
        [render_encoder setVertexBuffer:[[self buffer:S_Main] drawTap] 
                                 offset:0 
                                atIndex:tri_vertices_I];
        [render_encoder setVertexBuffer:[[self buffer:S_Aux0] drawTap] 
                                 offset:0 
                                atIndex:tri_pers_mat_I];
        [render_encoder drawPrimitives:MTLPrimitiveTypeTriangle 
                           vertexStart:0 
                           vertexCount:[[self buffer:S_Main] getVertexCount]];
        [render_encoder endEncoding];
    }
}

@end
