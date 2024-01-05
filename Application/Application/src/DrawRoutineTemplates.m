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
- (DynamicBuffer*) newDynamicBufferWithVertexSize:(NSUInteger)vertex_size
                                    vertexCount:(NSUInteger)vertex_count
                                    storageMode:(MTLResourceOptions)storage_mode
{
    DynamicBuffer* buffer = [[DynamicBuffer alloc] initWithDevice:_device
                                                       vertexSize:vertex_size
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
    MTLRenderPipelineDescriptor* _render_pipeline_descriptor;
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
        
        _render_pipeline_descriptor  = [MTLRenderPipelineDescriptor new];
        _render_pass_descriptor      = [MTLRenderPassDescriptor new];
    }
    return self;
}

// ==== Render Pipeline ====
- (void) setVertexFunction:(NSString*)vertex_name
          fragmentFunction:(NSString*)fragment_name
{
    id<MTLFunction> vertex_function = [_library newFunctionWithName:vertex_name];
    if (vertex_function == nil) {
        NSLog(@"Failed to create %@", vertex_name);
        return;
    }
    
    id<MTLFunction> fragment_function = [_library newFunctionWithName:fragment_name];
    if (fragment_function == nil) {
        NSLog(@"Failed to create %@", fragment_name);
        return;
    }
    
    _render_pipeline_descriptor.vertexFunction   = vertex_function;
    _render_pipeline_descriptor.fragmentFunction = fragment_function;
}

- (void) setPixelFormat:(MTLPixelFormat)pixel_format {
    _render_pipeline_descriptor.colorAttachments[0].pixelFormat = pixel_format;
}

- (void) setVertexBufferImmutable:(NSUInteger)index {
    _render_pipeline_descriptor.vertexBuffers[index].mutability = MTLMutabilityImmutable;
}

- (void) enableIndirectCommandBuffer {
    _render_pipeline_descriptor.supportIndirectCommandBuffers = YES;
}

- (id<MTLRenderPipelineState>) compileRenderPipeline {
    @autoreleasepool {
        NSError* error = nil;
        id<MTLRenderPipelineState> pipeline = [_device newRenderPipelineStateWithDescriptor:_render_pipeline_descriptor error:&error];
        NSAssert(pipeline, @"Failed to create Render Pipeline State: ", error);
        
        return pipeline;
    }
}


// ==== Compute Pipeline ====
- (id<MTLComputePipelineState>) computePipelineWithFunctionName:(NSString*)name {
    @autoreleasepool {
        id<MTLFunction> function = [_library newFunctionWithName:name];
        if (function == nil) {
            NSLog(@"Failed to create %@", name);
            return nil;
        }
        
        NSError* error = nil;
        id<MTLComputePipelineState> pipeline = [_device newComputePipelineStateWithFunction:function error:&error];
        NSAssert(pipeline, @"Failed to create Compute Pipeline State: ", error);
        
        return pipeline;
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
    [_render_pipeline_descriptor release];
}

@end


// ================ Null Draw Routine ================
@implementation NullDrawRoutine
- (nonnull instancetype) initWithDevice:(nonnull id<MTLDevice>)device
                                library:(nonnull id<MTLLibrary>)library
{
    self = [super init];
    return self;
}
- (void) configureWithDrawablePixelFormat:(MTLPixelFormat)pixel_format {}
- (void) createBufferWithVertexCount:(NSUInteger)count {}

- (void) bindBuffer:(NSUInteger)buffer_index {}
- (nullable id<MTLBuffer>) writeBufferOpen { return nil; }
- (void) writeBufferClose {}
- (void) drawInDrawable:(nonnull id<CAMetalDrawable>)drawable
        inCommandBuffer:(nonnull id<MTLCommandBuffer>)command_buffer {}
- (void) predrawOpenInBuffers {}
- (void) predrawCloseInBuffers {}
- (void) drawCompletedInBuffers {}

@end
