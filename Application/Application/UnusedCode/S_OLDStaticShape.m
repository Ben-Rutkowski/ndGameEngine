//#import "DrawRoutines.h"
//#import "shader_types.h"
//
//@implementation OLDStaticShapeSubroutine
//{
////    --- Pipelines ---
//    id<MTLRenderPipelineState> _new_pipeline_state;
//    
////    --- Resources ---
//    id<MTLBuffer> _vertex_data_buffer;
//    NSUInteger    _vertex_count;
//}
//
//// ==== Configuring ====
//- (void) configureWithDrawablePixelFormat:(MTLPixelFormat)pixel_format {
//    [self setVertexFunction:@"StaticShape_vertexShader"
//           fragmentFunction:@"StaticShape_fragmentShader"];
//    [self setPixelFormat:pixel_format];
//    [self setVertexBufferImmutable:StaticShape_VertexIndex_vertices];
//    _new_pipeline_state = [self compilePipeline];
//    
//    [self setClearColor:MTLClearColorMake(0.0, 0.5, 0.5, 1.0)];
//    [self finalizeRenderPass];
//    
//    [self finializeConfig];
//}
//
//// ==== Resources ====
//- (void) bindBuffer:(NSUInteger)index {}
//
//- (void) linkBufferOLD:(nonnull id<MTLBuffer>)buffer
//           vertexCount:(NSUInteger)count {
//    _vertex_data_buffer = buffer;
//    _vertex_count       = count;
//}
//
//- (void) linkBuffer:(DynamicBuffer*)buffer {
//    
//}
//
//// ==== Drawing ====
//- (void) encodeSubroutineInBuffer:(nonnull id<MTLCommandBuffer>)command_buffer
//                        inTexture:(nonnull id<MTLTexture>)texture
//{
//    MTLRenderPassDescriptor* current_pass = [self currentRenderPassDescriptor:texture];
//    
//    @autoreleasepool {
//        id<MTLRenderCommandEncoder> command_encoder = [command_buffer renderCommandEncoderWithDescriptor:current_pass];
//        
//        [command_encoder setRenderPipelineState:_new_pipeline_state];
//        
//        [command_encoder setVertexBuffer:_vertex_data_buffer
//                                  offset:0
//                                 atIndex:StaticShape_VertexIndex_vertices];
//        
//        float aspect_ratio = (float)600/(float)800;
//        
//        [command_encoder setVertexBytes:&aspect_ratio
//                                 length:sizeof(aspect_ratio)
//                                atIndex:StaticShape_VertexIndex_aspect_ratio];
//        
//        [command_encoder drawPrimitives:MTLPrimitiveTypeTriangle
//                            vertexStart:0
//                            vertexCount:_vertex_count];
//        
//        [command_encoder endEncoding];
//    }
//}
//
//@end
