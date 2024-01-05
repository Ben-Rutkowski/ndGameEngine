#import "DrawRoutineTemplates.h"


// ================ Draw Routine Template ================
@implementation DrawRoutineTemplate
{
    id<MTLDevice>       _device;
    id<MTLCommandQueue> _internal_blit_command_queue;
}

// ==== Configure ====
- (nonnull instancetype) initWithDevice:(nonnull id<MTLDevice>)device {
    self = [super init];
    if (self) {
        _device                      = device;
        _internal_blit_command_queue = [_device newCommandQueue];
    }
    return self;
}

// ==== Resources ====
- (DynamicBuffer*) newDynamicBufferWithDataSize:(NSUInteger)data_size
                                    vertexCount:(NSUInteger)vertex_count
                                    storageMode:(MTLResourceOptions)storage_mode
{
    DynamicBuffer* buffer = [[DynamicBuffer alloc] initWithDevice:_device
                                                         dataSize:data_size
                                                      vertexCount:vertex_count
                                                   andStorageMode:storage_mode];
    return buffer;
}

- (id<MTLCommandBuffer>) getBlitCommandBuffer {
    return [_internal_blit_command_queue commandBuffer];
}

@end


// ================ Draw Subroutine Template ================
@implementation DrawSubroutineTemplate
{
//    --- Hidden Metal Components ---
    id<MTLDevice>  _device;
    id<MTLLibrary> _library;
    
//    --- Hidden Render Components ---
    MTLRenderPipelineDescriptor* _pipeline_state_descriptor;
    MTLRenderPassDescriptor*     _render_pass_descriptor;
}

// ==== Configure ===
- (instancetype) initWithDevice:(id<MTLDevice>)device
                        library:(id<MTLLibrary>)library
{
    self = [super init];
    if (self) {
        _device  = device;
        _library = library;
        
        _pipeline_state_descriptor = [MTLRenderPipelineDescriptor new];
        _render_pass_descriptor    = [MTLRenderPassDescriptor new];
    }
    return self;
}

// ==== Pipeline ====
- (void) setVertexFunction:(NSString*)vertex_name
          fragmentFunction:(NSString*)fragment_name
{
    id<MTLFunction> vertex_function = [_library newFunctionWithName:vertex_name];
    if (vertex_function == nil) {
        NSLog(@"Failed to create %@", vertex_name);
    }
    
    id<MTLFunction> fragment_function = [_library newFunctionWithName:fragment_name];
    if (fragment_function == nil) {
        NSLog(@"Failed to create %@", fragment_name);
    }
    
    _pipeline_state_descriptor.vertexFunction   = vertex_function;
    _pipeline_state_descriptor.fragmentFunction = fragment_function;
}

- (void) setPixelFormat:(MTLPixelFormat)pixel_format {
    _pipeline_state_descriptor.colorAttachments[0].pixelFormat = pixel_format;
}

- (void) setVertexBufferImmutable:(NSUInteger)index {
    _pipeline_state_descriptor.vertexBuffers[index].mutability = MTLMutabilityImmutable;
}

- (void) enableIndirectCommandBuffer {
    _pipeline_state_descriptor.supportIndirectCommandBuffers = YES;
}

- (id<MTLRenderPipelineState>) compilePipeline {
    @autoreleasepool {
        NSError* error = nil;
        id<MTLRenderPipelineState> new_pipeline_state = [_device newRenderPipelineStateWithDescriptor:_pipeline_state_descriptor error:&error];
        NSAssert(new_pipeline_state, @"Failed to create Render Pipeline State: ", error);
        
        return new_pipeline_state;
    }
}

// ==== Render Pass ====
- (void) setClearColor:(MTLClearColor)color {
    _render_pass_descriptor.colorAttachments[0].loadAction = MTLLoadActionClear;
    _render_pass_descriptor.colorAttachments[0].clearColor = color;
}

- (void) finalizeRenderPass {
    _render_pass_descriptor.colorAttachments[0].storeAction = MTLStoreActionStore;
}

// ==== Indirect Command Buffer ====
- (id<MTLIndirectCommandBuffer>) setUpICBVertexBufferCount:(NSUInteger)vertex_count
                                       fragmentBufferCount:(NSUInteger)fragment_count
                                                maxCommand:(NSUInteger)max_command_count
{
    @autoreleasepool {
        MTLIndirectCommandBufferDescriptor* inderect_command_buffer_descriptor = [MTLIndirectCommandBufferDescriptor new];
        
        inderect_command_buffer_descriptor.commandTypes   = MTLIndirectCommandTypeDraw;
        inderect_command_buffer_descriptor.inheritBuffers = NO;
        inderect_command_buffer_descriptor.maxVertexBufferBindCount   = vertex_count;
        inderect_command_buffer_descriptor.maxFragmentBufferBindCount = fragment_count;
        
        id<MTLIndirectCommandBuffer> new_indirect_command_buffer = [_device newIndirectCommandBufferWithDescriptor:inderect_command_buffer_descriptor
                                                                   maxCommandCount:max_command_count options:0];
        return new_indirect_command_buffer;
    }
}

// ==== Draw ====
- (MTLRenderPassDescriptor*) currentRenderPassDescriptor:(id<MTLTexture>)texture {
    _render_pass_descriptor.colorAttachments[0].texture = texture;
    return _render_pass_descriptor;
}

// ==== Finalizing ====
- (void) finializeConfig {
    [_pipeline_state_descriptor release];
}

@end


// ================ Null Draw Routine ================
@implementation NullDrawRoutine
- (instancetype) initWithDevice:(nonnull id<MTLDevice>)device
                        library:(nonnull id<MTLLibrary>)library
{
    self = [super init];
    return self;
}

- (void) configureWithDrawablePixelFormat:(MTLPixelFormat)pixel_format {}
- (void) bindBuffer:(NSUInteger)buffer_index {}
- (void) createBufferWithVertexCount:(NSUInteger)count {}
- (void) expandBufferToSize:(NSUInteger)new_size {}
- (DynamicBuffer*)getBuffer { return nil; }
- (void) drawInDrawable:(nonnull id<CAMetalDrawable>)drawable
        inCommandBuffer:(nonnull id<MTLCommandBuffer>)command_buffer
{
//    NSLog(@"Null Draw");
}
- (void) beginPredrawStageInBuffers {}
- (void) endDrawStageInBuffers {}
- (void) beginDrawStageInBuffers {}
- (void) endPredrawStageInBuffers {}

- (void)writeBufferClose {}
- (id<MTLBuffer>)writeBufferOpen {return nil;}

@end
