# **Class** | Rapid Buffer
A buffer that is meant to be rewritten every frame.
```
@interface RapidBuffer : NSObject<BufferProtocol>
```

### Description
A buffer that is meant to be written to every frame and drawn every frame. Used for uniform data or small and rapidly changing variables. 

### Implementation
Keeps a revolving set of three `id<MTLBuffer>` (the maximum number of in flight frames that Metal uses). There are two counters that keep track of the `_next_to_draw` and `_next_to_write` indices. 

Before a write is performed, the `_in_flight_semaphore` is waited. After a draw has been completed, `_in_flight_semaphore` is signaled. This makes it so that the write will not rewrite any buffers that have been updated, but not drawn from yet. 

Upon each write, the `_next_to_write` index is incremented modulus 3. Upon each draw encoding completion `_next_to_draw` is incremented modulus 3 ONLY IF `_next_to_write` and `_next_draw` are different values. This allows the write index to be equal or ahead (mod 3) of the draw index. If a write has not been performed, the draw index will not increase. Furthermore, the write index will not be more than three steps ahead of the draw index because the `_in_flight_semaphore` will not permit it.



Writes and draws are protected by the `_in_flight_semaphore` of size 3 that permits at most three buffers to be written and drawn.

# Attributes
### device
Metal device.
```
id<MTLDevice> _device;
```

### buffers
The rotating array of buffers.
```
id<MTLBuffer> _buffers[3];
```

### vertex count
The number of vertices in the buffer.
```
NSUInteger _vertex_count;
```

### vertex size
The size of a single vertex.
```
NSUInteger _vertex_size;
```

### next to draw
The index of the next buffer that has been modified but not drawn.
```
NSUInteger _next_to_draw;
```

### next to write
The index of the next buffer that is available to be written.
```
NSUInteger _next_to_write;
```

### in flight semaphore
Semaphore of size 3. Waits at the beginning of every write and signals at the end of every draw completion.
```
dispatch_semaphore_t _in_flight_semaphore;
```
