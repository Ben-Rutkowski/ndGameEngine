#import "DrawRoutines.h"

@implementation DrawSubroutineTemplate
{
//    --- Temporary Render Configure Objects ---
    id<MTLDevice>                _device;
    id<MTLLibrary>               _library;
    
    MTLRenderPipelineDescriptor* _pipeline_state_descriptor;
}

- (instancetype) initWithDevice:(id<MTLDevice>)device
                        library:(id<MTLLibrary>)library
{
    NSLog(@"Init DrawSubroutine Template");
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
    _pipeline_state_descriptor.vertexFunction   = [_library newFunctionWithName:vertex_name];
    _pipeline_state_descriptor.fragmentFunction = [_library newFunctionWithName:fragment_name];
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

- (void) finializePipeline {
    @autoreleasepool {
        NSError* error = nil;
        _pipeline_state = [_device newRenderPipelineStateWithDescriptor:_pipeline_state_descriptor error:&error];
        NSAssert(_pipeline_state, @"Failed to create Render Pipeline State: ", error);
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
- (void) setUpICBVertexBufferCount:(NSUInteger)vertex_count
               fragmentBufferCount:(NSUInteger)fragment_count
                        maxCommand:(NSUInteger)max_command_count
{
    @autoreleasepool {
        MTLIndirectCommandBufferDescriptor* inderect_command_buffer_descriptor = [MTLIndirectCommandBufferDescriptor new];
        
        inderect_command_buffer_descriptor.commandTypes   = MTLIndirectCommandTypeDraw;
        inderect_command_buffer_descriptor.inheritBuffers = NO;
        inderect_command_buffer_descriptor.maxVertexBufferBindCount   = vertex_count;
        inderect_command_buffer_descriptor.maxFragmentBufferBindCount = fragment_count;
        
        _indirect_command_buffer = [_device newIndirectCommandBufferWithDescriptor:inderect_command_buffer_descriptor
                                                                   maxCommandCount:max_command_count options:0];
    }
}

// ==== Drawing ====
- (void) setDrawTexture:(nonnull id<MTLTexture>)texture {
    _render_pass_descriptor.colorAttachments[0].texture = texture;
}

// ==== Finalizing ====
- (void) finializeInit {
    [_device release];
    [_library release];
    [_pipeline_state_descriptor release];
}

@end
