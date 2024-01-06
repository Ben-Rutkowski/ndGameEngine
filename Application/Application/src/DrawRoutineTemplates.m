#import "DrawRoutineTemplates.h"

// ================ Draw Routine Template ================
@implementation DrawRoutineTemplate
{
    id<MTLDevice>       _device;
    id<MTLCommandQueue> _internal_blit_command_queue;
    
    NSMutableArray<DynamicBuffer*>* _buffers;
    NSUInteger _current_buffer;
}


- (instancetype) initWithDevice:(nonnull id<MTLDevice>)device
                numberOfBuffers:(NSUInteger)buffer_count
{
    self = [super init];
    if (self) {
        _device                      = device;
        _internal_blit_command_queue = [_device newCommandQueue];
        _buffers = [NSMutableArray arrayWithCapacity:buffer_count];
    }
    return self;
}


// ==== Resources ====
- (void)createBufferWithVertexSize:(NSUInteger)vertex_size 
                       vertexCount:(NSUInteger)vertex_count
                       storageMode:(MTLResourceOptions)storage_mode
{
    _buffers[_current_buffer] = [[DynamicBuffer alloc] 
                                 initWithDevice:_device
                                 vertexSize:vertex_size
                                 vertexCount:vertex_count
                                 andStorageMode:storage_mode];
}

- (void) bindBuffer:(NSUInteger)index {
    NSLog(@"Bind Buffer : %lu", index);
    _current_buffer = index;
}

- (id<MTLBuffer>) writeBufferOpen {
    return nil;
}

- (void) writeBufferClose {
    
}

- (id<MTLCommandBuffer>) getBlitCommandBuffer {
    return [_internal_blit_command_queue commandBuffer];
}


// ==== Draw ====
- (void)predrawOpenInBuffers {
    for (int i=0; i<_buffers.count; i++) {
        [_buffers[i] predrawOpen];
    }
}

- (void)predrawCloseInBuffers {
    for (int i=0; i<_buffers.count; i++) {
        [_buffers[i] predrawClose];
    }
}

- (void)drawCompletedInBuffers {
    for (int i=0; i<_buffers.count; i++) {
        [_buffers[i] drawCompleted];
    }
}


// ==== Depricated ====
- (DynamicBuffer*) newDynamicBufferWithVertexSizeOLD:(NSUInteger)vertex_size
                                      vertexCount:(NSUInteger)vertex_count
                                      storageMode:(MTLResourceOptions)storage_mode
{
    DynamicBuffer* buffer = [[DynamicBuffer alloc] initWithDevice:_device
                                                       vertexSize:vertex_size
                                                      vertexCount:vertex_count
                                                   andStorageMode:storage_mode];
    return buffer;
}

@end


// ================ Draw Subroutine Template ================
@implementation DrawSubroutineTemplate
{
//    --- Hidden Metal Components ---
    id<MTLDevice>  _hidden_device;
    
    DynamicBuffer* _buffer[5];
    SubroutineEnum _current_index;
    
//    --- Hidden Render Components ---
    MTLRenderPipelineDescriptor* _render_pipeline_descriptor;
    MTLRenderPassDescriptor*     _render_pass_descriptor;
    
//    --- Depricated ---
    id<MTLLibrary> _library;
}


// ==== Configure ===
- (instancetype) initWithDevice:(id<MTLDevice>)device
                        library:(id<MTLLibrary>)library
{
    self = [super init];
    if (self) {
        _hidden_device  = device;
        
        _render_pipeline_descriptor  = [MTLRenderPipelineDescriptor new];
        _render_pass_descriptor      = [MTLRenderPassDescriptor new];
        
//        --- Depricated ---
        _library = library;
    }
    return self;
}

- (id<MTLBuffer>) newAuxBufferOfSize:(NSUInteger)data_size {
    return [_hidden_device newBufferWithLength:data_size
                                options:MTLResourceStorageModePrivate];
}


// ==== Resources ====
- (void) bindBuffer:(SubroutineEnum)index {
    _current_index = index;
}

- (void) linkBuffer:(DynamicBuffer*)buffer {
    _buffer[_current_index] = buffer;
}

- (DynamicBuffer*) buffer:(SubroutineEnum)index {
    return _buffer[index];
}


// ==== Render Pipeline ====
- (void) renderSetVertexFunction:(NSString*)vertex_name
          fragmentFunction:(NSString*)fragment_name
                   library:(nonnull id<MTLLibrary>)library
{
    id<MTLFunction> vertex_function = [library newFunctionWithName:vertex_name];
    if (vertex_function == nil) {
        NSLog(@"Failed to create %@", vertex_name);
        return;
    }
    
    id<MTLFunction> fragment_function = [library newFunctionWithName:fragment_name];
    if (fragment_function == nil) {
        NSLog(@"Failed to create %@", fragment_name);
        return;
    }
    
    _render_pipeline_descriptor.vertexFunction   = vertex_function;
    _render_pipeline_descriptor.fragmentFunction = fragment_function;
}

- (void) renderSetPixelFormat:(MTLPixelFormat)pixel_format {
    _render_pipeline_descriptor.colorAttachments[0].pixelFormat = pixel_format;
}

- (void) renderSetVertexBufferImmutable:(NSUInteger)index {
    _render_pipeline_descriptor.vertexBuffers[index].mutability = MTLMutabilityImmutable;
}

- (void) renderEnableIndirectCommandBuffer {
    _render_pipeline_descriptor.supportIndirectCommandBuffers = YES;
}

- (id<MTLRenderPipelineState>) compileRenderPipeline {
    @autoreleasepool {
        NSError* error = nil;
        id<MTLRenderPipelineState> pipeline = [_hidden_device newRenderPipelineStateWithDescriptor:_render_pipeline_descriptor error:&error];
        NSAssert(pipeline, @"Failed to create Render Pipeline State: ", error);
        
        return pipeline;
    }
}


// ==== Compute Pipeline ====
- (id<MTLComputePipelineState>) computePipelineWithFunctionName:(NSString*)name
                                                        library:(nonnull id<MTLLibrary>)library
{
    @autoreleasepool {
        id<MTLFunction> function = [library newFunctionWithName:name];
        if (function == nil) {
            NSLog(@"Failed to create %@", name);
            return nil;
        }
        
        NSError* error = nil;
        id<MTLComputePipelineState> pipeline = [_hidden_device newComputePipelineStateWithFunction:function error:&error];
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
        
        id<MTLIndirectCommandBuffer> new_indirect_command_buffer = [_hidden_device newIndirectCommandBufferWithDescriptor:inderect_command_buffer_descriptor
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


// ==== Depricated ====
- (void) setVertexFunctionOLD:(NSString*)vertex_name
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

- (id<MTLComputePipelineState>) computePipelineWithFunctionNameOLD:(NSString*)name
{
    @autoreleasepool {
        id<MTLFunction> function = [_library newFunctionWithName:name];
        if (function == nil) {
            NSLog(@"Failed to create %@", name);
            return nil;
        }
        
        NSError* error = nil;
        id<MTLComputePipelineState> pipeline = [_hidden_device newComputePipelineStateWithFunction:function error:&error];
        NSAssert(pipeline, @"Failed to create Compute Pipeline State: ", error);
        
        return pipeline;
    }
}

@end


// ================ Null Draw Routine ================
@implementation NullDrawRoutine
- (nonnull instancetype) initWithDeviceOLD:(nonnull id<MTLDevice>)device
                                library:(nonnull id<MTLLibrary>)library
{
    self = [super init];
    return self;
}
- (void) configureWithDrawablePixelFormatOLD:(MTLPixelFormat)pixel_format {}
- (void) createBufferWithVertexCountOLD:(NSUInteger)count {}

- (void) bindBufferOLD:(NSUInteger)buffer_index {}
- (nullable id<MTLBuffer>) writeBufferOpenOLD { return nil; }
- (void) writeBufferCloseOLD {}
- (void) drawInDrawable:(nonnull id<CAMetalDrawable>)drawable
        inCommandBuffer:(nonnull id<MTLCommandBuffer>)command_buffer {}
- (void) predrawOpenInBuffersOLD {}
- (void) predrawCloseInBuffersOLD {}
- (void) drawCompletedInBuffersOLD {}

- (nonnull instancetype)initWithDevice:(nonnull id<MTLDevice>)device library:(nonnull id<MTLLibrary>)library pixelFormat:(MTLPixelFormat)pixel_format { 
    self = [super init];
    return self;
}

- (void)bindBuffer:(NSUInteger)index {
}


- (void)createBufferWithVertexSize:(NSUInteger)vertex_size vertexCount:(NSUInteger)vertex_count storageMode:(MTLResourceOptions)storage_mode { 
}



@end
