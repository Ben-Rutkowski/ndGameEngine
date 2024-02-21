# **Class** | DynamicBuffer
Buffer that can be drawn from quickly and is able to be written to often but not every frame.
```
@interface DynamicBuffer : NSObject<BufferProtocol>
```

### Description
A virtual buffer implementation that conforms to `BufferProtocol`. Keeps two revolving `id<MTLBuffer>`, a front and back buffer. When no outstanding write has been performed, the `DynamicBuffer` will draw from its front buffer. When a write has been opened, it will allow modification of its back buffer. When the write is closed, it will copy the buffer contents back to front. While it is waiting to transfer back to front, the buffer will temporarily draw from the back buffer. When the transfer is completed, it will resume drawing from the front buffer. You will not be allowed to open for writing again until a complete transfer has been performed.

# Attributes
### device
Metal device.
```
id<MTLDevice> _device;
```

### command_queue
A command queue to perform blit commands on. The same command queue that the draw routine owns.
```
id<MTLCommandQueue> _command_queue;
```

### buffer
The front and back buffers; indexed by `FrontBuffer`, `BackBuffer`.
```
id<MTLBuffer> _buffer[2];
```

### working buffer
Set to the latest drawable buffer on `predrawOpen`; set to `nil` on `predrawClose`.
```
id<MTLBuffer> _working_buffer;
```

### vertex count
Keeps track of the number of vertices in the front and back buffers; used for resize mechanics.
```
NSUInteger _vertex_count[2];
```

### reference count
Keeps track of the number of in-flight frames for each buffer.
```
NSUInteger _refernce_count[2];
```

### current index
Either `FrontBuffer` or `BackBuffer`; the current buffer to be drawn from.
```
NSUInteger _current_index; 
```

### active index
Either `FrontBuffer` or `BackBuffer`; the buffer that was referenced by the next to be completed in-flight frame.
```
NSUInteger _active_index;
```

### vertex size
The size of a single vertex stored in the buffer.
```
NSUInteger _vertex_size;
```

### swap phase
An enum that keeps track of what phase the buffer is in; takes the values `WriteComplete_EncodeFront_DrawFront`, `EncodingBack_DrawingFront`, `EncodingFront_DrawingBack`, `EncodingBack_DrawingBack`.
```
SwapPhase _swap_phase;
```

### index swap semaphore
Binary semaphore; is set during any operation that modifies the `active_index`and reference count changes that use the `active_index`.
```
dispatch_semaphore_t _index_swap_semaphore;
```

### encode on queue semaphore
Binary semaphore; is set during any operation that modifies the `current_index` and reference count changes that use the `current_index`.
```
dispatch_semaphore_t _encode_on_queue_semaphore;
```


### complete swap semaphore
Binary semaphore; set when a write is completed, and signaled when the swap has been completed and a new write can be opened.
```
dispatch_semaphore_t _complete_swap_semaphore;
```
