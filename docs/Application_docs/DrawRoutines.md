# **Protocol** | Draw Subroutine Protocol
Draw Subroutines take buffers as registers and preform a single draw step, *e.g.* drawing triangles to the screen.
```
@protocol DrawSubroutineProtocol
```

### Description
A protocol for interfacing with a draw sub routine. Draw subroutines are owned by draw routines. In the initialization of a `DrawSubroutineProtocol`, all of the necessary pipeline states must be created. And in the encoding stage, the specific subroutine is encoded onto a texture. Any draw routine that owns a subroutine will first attach buffers to the subroutine and then encode into it.

# Methods
### Initialize
Creates the necessary pipeline state objects for the subroutine.
```
- (nonnull instancetype) initWithDevice:(nonnull id<MTLDevice>)device
                                library:(nonnull id<MTLLibrary>)library
                            pixelFormat:(MTLPixelFormat)pixel_format;
```

### Encode Subroutine
Encodes the subroutine in a given command buffer to draw in a given texture.
```
- (void) encodeSubroutineInBuffer:(nonnull id<MTLCommandBuffer>)command_buffer
                        inTexture:(nonnull id<MTLTexture>)texture;
```

# **Protocol** | Draw Routine Protocol
An interface for a draw routine which is owned by the `RenderSwitcher`.
```
- (void) drawInDrawable:(nonnull id<CAMetalDrawable>)drawable
        inCommandBuffer:(nonnull id<MTLCommandBuffer>)command_buffer;
```

### Description
Owns a collection of subroutines. Must initialize each draw subroutine in the initialization. The `RenderSwitcher` delivers a drawable and command buffer upon every frame draw and calls. Here the draw routine performs its full routine, by binding buffers to subroutine registers, encoding subroutines in the order of execution.

# Methods
### Initialize
Calls the `DrawRoutineTemplate` initialization and sets the number of buffers. Initializes all subroutines.
```
- (nonnull instancetype) initWithDevice:(nonnull id<MTLDevice>)device
                           commandQueue:(nonnull id<MTLCommandQueue>)command_queue
                                library:(nonnull id<MTLLibrary>)library
                            pixelFormat:(MTLPixelFormat)pixel_format;
```

### Draw in Drawable and Command Buffer
Called by the `RenderSwitcher`, represents the entire draw routine.
```
- (void) drawInDrawable:(nonnull id<CAMetalDrawable>)drawable
        inCommandBuffer:(nonnull id<MTLCommandBuffer>)command_buffer;
```
