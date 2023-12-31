#import "DrawRoutines.h"
#import "shader_types.h"

@implementation BasicTriangleDrawRoutine
{
    id<MTLRenderPipelineState> _render_pipeline_state;
}

- (nonnull instancetype) initWithMTKView:(nonnull MTKView*)mtk_view MTLLibrary:(nonnull id<MTLLibrary>)library {
    self = [super init];
    if (self) {
        @autoreleasepool {
            NSLog(@"Initializing BasicTriangleDrawRoutine");
            
            NSError* error = nil;
            MTLRenderPipelineDescriptor* pipeline_descriptor = [[MTLRenderPipelineDescriptor alloc] init];
            pipeline_descriptor.rasterSampleCount = mtk_view.sampleCount;
            pipeline_descriptor.colorAttachments[0].pixelFormat = mtk_view.colorPixelFormat;
            pipeline_descriptor.vertexFunction   = [library newFunctionWithName:@"BasicTriangle_vertexShader"];
            pipeline_descriptor.fragmentFunction = [library newFunctionWithName:@"BasicTriangle_fragmentShader"];
            pipeline_descriptor.vertexBuffers[BasicTriangle_VertexIndexVertices].mutability = MTLMutabilityImmutable;
            _render_pipeline_state = [mtk_view.device newRenderPipelineStateWithDescriptor:pipeline_descriptor error:&error];
            NSAssert(_render_pipeline_state, @"Failed to Create Pipeline for Basic Triangle Routine", error);
        }
    }
    
    return self;
}

- (void) commitWithMTKView:(nonnull MTKView*)mtk_view CommandQueue:(nonnull id<MTLCommandQueue>)command_queue {
    NSLog(@"Drawing Basic Triangle");
    
    static const BasicTriangle_VertexType vertices[] = {
        { {  0.5,  -0.5 },  { 0.3, 0.6, 0.7, 1.0 } },
        { { -0.5,  -0.5 },  { 0.5, 0.6, 0.7, 1.0 } },
        { {  0.0,   0.5 },  { 0.7, 0.6, 0.7, 1.0 } }
    };
    
    @autoreleasepool {
        MTLRenderPassDescriptor* render_pass_descriptor = mtk_view.currentRenderPassDescriptor;
        if (render_pass_descriptor) {
            id<MTLCommandBuffer> command_buffer = [command_queue commandBuffer];
            
            //        Commands
            id<MTLRenderCommandEncoder> command_encoder = [command_buffer renderCommandEncoderWithDescriptor:render_pass_descriptor];
            [command_encoder setRenderPipelineState:_render_pipeline_state];
            
            [command_encoder setVertexBytes:vertices
                                     length:sizeof(vertices)
                                    atIndex:BasicTriangle_VertexIndexVertices];
            
            [command_encoder drawPrimitives:MTLPrimitiveTypeTriangle
                                vertexStart:0
                                vertexCount:3];
            
            [command_encoder endEncoding];
            
            //        Commit in Drawable
            [command_buffer presentDrawable:mtk_view.currentDrawable];
            [command_buffer commit];
            [command_buffer waitUntilCompleted];
        }
    }
}

@end
