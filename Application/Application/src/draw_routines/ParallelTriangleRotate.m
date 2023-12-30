#import "DrawRoutines.h"
#import "shader_types.h"
#import <QuartzCore/CAMetalLayer.h>

//static const NSUInteger MAX_FRAMES_IN_FLIGHT = 3;
//
//const ParallelTriangleRotate_vertexType triangle_vertices[] = {
//    { { -0.5, -0.5 }, { 0.0, 1.0, 1.0, 1.0 } },
//    { {  0.5, -0.5 }, { 1.0, 1.0, 1.0, 1.0 } },
//    { {  0.0,  0.5 }, { 0.5, 1.0, 1.0, 1.0 } }
//};
//
//@implementation ParallelTriangleRotateDrawRoutine
//{
//    dispatch_semaphore_t _in_flight_semaphore;
//    
//    id<MTLBuffer> _vertex_buffers[MAX_FRAMES_IN_FLIGHT];
//    NSUInteger    _current_buffer;
//    float         _aspect_ratio;
//    float         _theta;
//    
//    id<MTLRenderPipelineState> _render_pipeline_state;
//}
//
//- (nonnull instancetype)initWithMTKView:(nonnull MTKView *)mtk_view MTLLibrary:(nonnull id<MTLLibrary>)library {
//    self = [super init];
//    if (self) {
//        @autoreleasepool {
//            NSLog(@"Initializing ParallelTriangleRotateDrawRoutine");
//            
////            Set up Pipeline
//            NSError* error = nil;
//            MTLRenderPipelineDescriptor* pipeline_descriptor = [[MTLRenderPipelineDescriptor alloc] init];
//            pipeline_descriptor.rasterSampleCount = mtk_view.sampleCount;
//            pipeline_descriptor.colorAttachments[0].pixelFormat = mtk_view.colorPixelFormat;
//            pipeline_descriptor.vertexFunction   = [library newFunctionWithName:@"ParallelTriangleRotate_vertexShader"];
//            pipeline_descriptor.fragmentFunction = [library newFunctionWithName:@"ParallelTriangleRotate_fragmentShader"];
//            pipeline_descriptor.vertexBuffers[ParallelTriangleRotate_vertexIndexVertices].mutability = MTLMutabilityImmutable;
//            _render_pipeline_state = [mtk_view.device newRenderPipelineStateWithDescriptor:pipeline_descriptor error:&error];
//            NSAssert(_render_pipeline_state, @"Failed to Create Pipeline for Parallel Triangle Rotate Routine", error);
//            
////            Set up Buffers
//            for (NSUInteger i=0; i<MAX_FRAMES_IN_FLIGHT; i++) {
//                _vertex_buffers[i] = [mtk_view.device newBufferWithLength:sizeof(triangle_vertices)
//                                                                  options:MTLResourceStorageModeShared];
//            }
//            
////            Init Semaphore
//            _in_flight_semaphore = dispatch_semaphore_create(MAX_FRAMES_IN_FLIGHT);
//            
//            _current_buffer = 0;
//            _theta = 0.0;
//        }
//    }
//    
//    return self;
//}
//
//- (void) updateViewPortWithSize:(CGSize)size {    
//    _aspect_ratio = (float)size.height/(float)size.width;
//    
////    NSLog(@"Aspect Ratio: %f", _aspect_ratio);
//    
//    [self updateState];
//}
//
//- (void) updateState {
//    const float rotate_speed = 0.02;
//    
//    _theta += rotate_speed;
//    
//    ParallelTriangleRotate_vertexType* current_vertices = _vertex_buffers[_current_buffer].contents;
//    
//    vector_float2 bottom_left  = triangle_vertices[0].position;
//    vector_float2 bottom_right = triangle_vertices[1].position;
//    vector_float2 top          = triangle_vertices[2].position;
//    
//    vector_float2 new_bottom_left;
//    vector_float2 new_bottom_right;
//    vector_float2 new_top;
//    
//    new_bottom_left[0] = cos(_theta)*bottom_left[0] - sin(_theta)*bottom_left[1];
//    new_bottom_left[1] = sin(_theta)*bottom_left[0] + cos(_theta)*bottom_left[1];
//    
//    new_bottom_right[0] = cos(_theta)*bottom_right[0] - sin(_theta)*bottom_right[1];
//    new_bottom_right[1] = sin(_theta)*bottom_right[0] + cos(_theta)*bottom_right[1];
//    
//    new_top[0] = cos(_theta)*top[0] - sin(_theta)*top[1];
//    new_top[1] = sin(_theta)*top[0] + cos(_theta)*top[1];
//    
//    current_vertices[0].position = new_bottom_left;
//    current_vertices[0].color    = triangle_vertices[0].color;
//    
//    current_vertices[1].position = new_bottom_right;
//    current_vertices[1].color    = triangle_vertices[1].color;
//    
//    current_vertices[2].position = new_top;
//    current_vertices[2].color    = triangle_vertices[2].color;
//}
//
//- (void)commitWithMTKView:(nonnull MTKView *)mtk_view CommandQueue:(nonnull id<MTLCommandQueue>)command_queue {
//    dispatch_semaphore_wait(_in_flight_semaphore, DISPATCH_TIME_FOREVER);
////    NSLog(@"Begin");
//
//    _current_buffer = (_current_buffer+1)%MAX_FRAMES_IN_FLIGHT;
//    
//    [self updateState];
//    
//    @autoreleasepool {
//        MTLRenderPassDescriptor* render_pass_descriptor = mtk_view.currentRenderPassDescriptor;
//        if (render_pass_descriptor != nil) {
//            id<MTLCommandBuffer> command_buffer = [command_queue commandBuffer];
//            
////            Command Encoding
//            id<MTLRenderCommandEncoder> render_encoder = [command_buffer renderCommandEncoderWithDescriptor:render_pass_descriptor];
//            [render_encoder setRenderPipelineState:_render_pipeline_state];
////            
//            [render_encoder setVertexBuffer:_vertex_buffers[_current_buffer]
//                                     offset:0
//                                    atIndex:ParallelTriangleRotate_vertexIndexVertices];
//            
//            [render_encoder setVertexBytes:&_aspect_ratio
//                                    length:sizeof(_aspect_ratio)
//                                   atIndex:ParallelTriangleRotate_vertexIndexAspectRatio];
//            
//            [render_encoder drawPrimitives:MTLPrimitiveTypeTriangle
//                               vertexStart:0
//                               vertexCount:3];
////            
//            [render_encoder endEncoding];
//
////            Commit in Drawable
//            [command_buffer presentDrawable:mtk_view.currentDrawable];
//            
//            __block dispatch_semaphore_t block_semaphore = _in_flight_semaphore;
//            [command_buffer addCompletedHandler:^(id<MTLCommandBuffer> buffer) {
////                NSLog(@"End");
//                dispatch_semaphore_signal(block_semaphore);
//            }];
//            
//            [command_buffer commit];
//        }
//    }
//}
//
//@end
