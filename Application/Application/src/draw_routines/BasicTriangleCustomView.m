#import "DrawRoutines.h"
#import "shader_types.h"

@implementation BasicTriangleCustomViewDrawRoutine
{
    id<MTLDevice> _device;
    id<MTLRenderPipelineState> _render_pipeline_state;
    
    MTLRenderPassDescriptor* _drawable_render_pass_descriptor;
    
}

- (nonnull instancetype)initWithDevice:(nonnull id<MTLDevice>)device MTLLibrary:(nonnull id<MTLLibrary>)library drawablePixelFormat:(MTLPixelFormat)drawable_pixel_format {
    NSLog(@"Initializing Basic Triangle Custom View Draw Routine");
    
    self = [super init];
    if (self) {
        _device = device;
        
        NSError* error = nil;
        MTLRenderPipelineDescriptor* pipeline_descriptor = [[MTLRenderPipelineDescriptor alloc] init];
        pipeline_descriptor.colorAttachments[0].pixelFormat = drawable_pixel_format;
        pipeline_descriptor.vertexFunction   = [library newFunctionWithName:@"BasicTriangle_vertexShader"];
        pipeline_descriptor.fragmentFunction = [library newFunctionWithName:@"BasicTriangle_fragmentShader"];
        pipeline_descriptor.vertexBuffers[BasicTriangle_VertexIndexVertices].mutability = MTLMutabilityImmutable;
        _render_pipeline_state = [_device newRenderPipelineStateWithDescriptor:pipeline_descriptor error:&error];
        NSAssert(_render_pipeline_state, @"Failed to Create Pipeline for Basic Triangle Routine", error);
        
        _drawable_render_pass_descriptor = [MTLRenderPassDescriptor new];
        _drawable_render_pass_descriptor.colorAttachments[0].loadAction  = MTLLoadActionClear;
        _drawable_render_pass_descriptor.colorAttachments[0].storeAction = MTLStoreActionStore;
        _drawable_render_pass_descriptor.colorAttachments[0].clearColor  = MTLClearColorMake(0.0, 0.5, 0.5, 1.0);
    }
    
    return self;
}

- (void)commitIntoMetalLayer:(nonnull CAMetalLayer*)metal_layer commandQueue:(nonnull id<MTLCommandQueue>)command_queue {
    static const BasicTriangle_VertexType vertices[] = {
        { {  0.5,  -0.5 },  { 0.3, 0.6, 0.7, 1.0 } },
        { { -0.5,  -0.5 },  { 0.5, 0.6, 0.7, 1.0 } },
        { {  0.0,   0.5 },  { 0.7, 0.6, 0.7, 1.0 } }
    };

    @autoreleasepool {
        id<CAMetalDrawable> current_drawable = [metal_layer nextDrawable];
        if (!current_drawable) {
            NSLog(@"NO DRAWABLE");
            return;
        }
        
        _drawable_render_pass_descriptor.colorAttachments[0].texture = current_drawable.texture;
        
//        Commands
        id<MTLCommandBuffer> command_buffer = [command_queue commandBuffer];
        id<MTLRenderCommandEncoder> command_encoder = [command_buffer renderCommandEncoderWithDescriptor:_drawable_render_pass_descriptor];
        [command_encoder setRenderPipelineState:_render_pipeline_state];

        [command_encoder setVertexBytes:vertices
                                 length:sizeof(vertices)
                                atIndex:BasicTriangle_VertexIndexVertices];

        [command_encoder drawPrimitives:MTLPrimitiveTypeTriangle
                            vertexStart:0
                            vertexCount:3];

        [command_encoder endEncoding];

        //        Commit in Drawable
        [command_buffer presentDrawable:current_drawable];
        [command_buffer commit];
//        [command_buffer waitUntilCompleted];
    }
    
}

@end
