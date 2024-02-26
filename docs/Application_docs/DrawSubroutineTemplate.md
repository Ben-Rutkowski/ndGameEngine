# **Class** | Draw Subroutine Template
Encodes a specific step of a draw routine.
```
@interface DrawSubroutineTemplate : NSObject
```

### Description
A Draw Subroutine encodes a specific step in a draw routine and are owned by a Draw Routine. They often have one or two shaders associated with it. It holds a collection of IDs `buffer` for `id<BufferProtocol>` registers that are used in encoding the subroutine. When a Draw Subroutine is encoded in the draw step of a Draw Routine, specific `id<BufferProtocol>`s must be set to the `buffer`s of the Draw Subroutine before encoding the step. During initialization, the `id<MTLRenderPipelineState>` and `id<MTLComputePipelineState>` objects needed for the subroutine are created and abstracted using methods of the `DrawSubroutineTemplate`.

# Methods
## Resources
### Link Buffer At Index
Sets a `id<BufferProtocol>` to a buffer register in the subroutine at draw time.
```
- (void) linkBuffer:(nonnull id<BufferProtocol>)buffer
            atIndex:(SubroutineEnum)index;
```

### Buffer 
Returns the buffer and the given register index.
```
- (nonnull id<BufferProtocol>) buffer:(SubroutineEnum)index;
```

## Render Pipeline
### Render: Set Shader Functions 
Sets the temporary `MTLRenderPipelineDescriptor` to use the given shaders from the given library.
```
- (void) renderSetVertexFunction:(nonnull NSString*)vertex_name
          fragmentFunction:(nonnull NSString*)fragment_name
                   library:(nonnull id<MTLLibrary>)library;
```

### Render: Set Pixel Format
Sets the temporary `MTLRenderPipelineDescriptor` to use the given pixel format.
```
- (void) renderSetPixelFormat:(MTLPixelFormat)pixel_format;
```

### Render: Set Vertex Buffer Immutable
Sets the given vertex buffer as immutable. Want to do this as much as possible to improve performance.
```
- (void) renderSetVertexBufferImmutable:(NSUInteger)index;
```

### Render: Enable Indirect Command Buffer
Allows the pipeline state to use indirect command buffers.
```
- (void) renderEnableIndirectCommandBuffer;
```

### Compile Render Pipeline
Returns a finalized `id<MTLRenderPipelineState>` with the set configuration.
```
- (nullable id<MTLRenderPipelineState>) compileRenderPipeline;
```

## Compute Pipeline
### Compute Pipeline With Function Name
Returns a `id<MTLComputePipelineState>` that uses the given compute function.
```
- (nullable id<MTLComputePipelineState>) computePipelineWithFunctionName:(nonnull NSString*)name
                                                                 library:(nonnull id<MTLLibrary>)library;
```

## Render Pass
### Set Clear Color
Sets the load action of the `MTLRenderPassDescriptor` to `MTLLoadActionClear` and sets the clear color to the given color (all on color attachment 0).
```
- (void) setClearColor:(MTLClearColor)color;
```

### Finalize Render Pass
Sets the store action of the `MTLRenderPassDescriptor` to `MTLStoreActionStore`.
```
- (void) finalizeRenderPass;
```

## Indirect Command Buffer --WIP--

## Finalizing
### Finalize Config
Releases the `MTLRenderPipelineDescriptor`, as it is not needed after initialization.
```
- (void) finializeConfig;
```

## Draw
### Current Render Pass Descriptor
Returns a `MTLRenderPassDescriptor` that will draw into the given texture.
```
- (nonnull MTLRenderPassDescriptor*) currentRenderPassDescriptor:(nonnull id<MTLTexture>)texture;
```

# Attributes
### hidden device
The Metal device owned by the `RenderSwitcher`.
```
id<MTLDevice> _hidden_device;
```

### buffer
An array of five `id<BufferProtocol>` registers indexed by `S_Main`, `S_Aux0`, `S_Aux1`, `S_Aux2`, `S_Aux3`.
```
id<BufferProtocol> _buffer[5];
```

### render pipeline descriptor
Temporary descriptor used in the abstraction of initializing the pipeline states.
```
MTLRenderPipelineDescriptor* _render_pipeline_descriptor;
```

### render pass descriptor
Temporary descriptor used in the abstraction of the initialization of the render pass.
```
MTLRenderPassDescriptor* _render_pass_descriptor;
```
