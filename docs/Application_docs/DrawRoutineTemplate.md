# **Class** | Draw Routine Template
Executes the entire sequence of steps in a frame draw. Owns every buffer used to draw in that routine.
```
@interface DrawRoutineTemplate : NSObject
```

### Description
A `DrawRoutineTemplate` is a parent class for any draw routine. A draw routine contains the entire routine of every frame draw. That is, the `drawInDrawable:inCommandBuffer:` function is called once on every frame.

The `DrawRoutineTemplate` keeps some arrays of `id<BufferProtocol>` (dynamic, static, and rapid) which are controlled by binding them using an ID. When the `RenderSwitcher` calls the armed routine to draw, it calls `predrawOpenInBuffers` and `predrawCloseInBuffers`. The `DrawRoutineTemplate` will then call the corresponding functions (`predrawOpen` and `predrawClose`) on each of the buffers in the array.

# Methods
## Resources
### Create Buffer
Creates a `id<BufferProtocol>` and adds it to the array of buffers `buffers`.
```
- (void) createBufferWithVertexSize:(NSUInteger)vertex_size
                        vertexCount:(NSUInteger)vertex_count;
```

### Bind Buffer
Sets the current bound buffer.
```
- (void) bindBuffer:(NSUInteger)index;
```

### Buffer At
Returns the buffer at the given index.
```
- (id<BufferProtocol>) bufferAt:(NSUInteger)index;
```

### Write Buffer Open
Opens the bound buffer to write and returns the ID.
```
- (id<MTLBuffer>) writeBufferOpen;
```

### Write Buffer Close
Closes the bound buffer to write.
```
- (void) writeBufferClose;
```

## Draw
### Predraw Open In Buffers
Opens every `DynamicBuffer` in the draw routine to draw.
```
- (void) predrawOpenInBuffers;
```

### Predraw Close In Buffers
Closes every `DynamicBuffer` in the draw routine to draw.
```
- (void) predrawCloseInBuffers;
```

### Draw Complete In Buffers
Calls the draw complete method in every `DynamicBuffer` in the draw routine.
```
- (void) drawCompletedInBuffers;
```

# Attributes
### device
The Metal device owned by the `RenderSwitcher`
```
id<MTLDevice> _device;
```

### command queue
The Metal command queue owned by the `RenderSwitcher`
```
id<MTLCommandQueue> _command_queue;
```

### dynamic buffers
The array of dynamic buffers used in the draw routine.
```
NSMutableArray<id<BufferProtocol>>* _dynamic_buffers;
```

### current buffer
The current bound buffer index.
```
NSUInteger _current_buffer;
```
