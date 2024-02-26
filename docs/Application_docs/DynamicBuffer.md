# **Class** | Dynamic Buffer
Buffer that can be drawn from quickly and is able to be written to often but not every frame.
```
@interface DynamicBuffer : NSObject<BufferProtocol>
```

### Description
A virtual buffer implementation that conforms to `BufferProtocol`. Keeps two revolving `id<MTLBuffer>`, a front and back buffer. When no outstanding write has been performed, the `DynamicBuffer` will draw from its front buffer. When a write has been opened, it will allow modification of its back buffer. When the write is closed, it will copy the buffer contents back to front. While it is waiting to transfer back to front, the buffer will temporarily draw from the back buffer. When the transfer is completed, it will resume drawing from the front buffer. You will not be allowed to open for writing again until a complete transfer has been performed.

### Implementation
The `DynamicBuffer` keeps track of how many in-flight frames exists for each the front and back buffers. On each `predrawOpen` the most recent completed buffer counter is incremented. After a frame has been completed, the buffer counter of the buffer bound to that frame is decremented. Theses are counted in `reference_counter`. The buffer uses `current_buffer` and `active_buffer` (which take the values `FrontBuffer`, `BackBuffer` and index `reference_counter`) which at any point in time, represent the buffer to encode from, and the buffer that the next draw command completed is bounded to from, respectively. 

There are four phases that happen in each write. When a write is first completed and the new contents are written to the `BackBuffer`, the buffer starts encoding draws from the back. There may be current in-flight frames from the front buffer. If there are, the current phase is set to `EncodeBack_drawFront`, if there are no in-flight frames from the front, the current phase is set to `EncodeBack_drawBack` and a swap is initiated. If the phase is still `EncodeBack_drawFront`, at the end of each draw complete, it is check to see if there are any in-flight frames in the front buffer, if there are none, switch to `EncodeBack_drawBack` and initiate a swap. When a swap is encoded, the contents of the back buffer are blit to the front. When a swap is completed, the buffer starts to draw from the `FrontBuffer` again. It also checks if there are any remaining in-flight frames from the back buffer. If there are it sets the phase to `EncodeFront_DrawBack`, if not it sets the phase to `WriteComplete_EncodeFront_DrawFront`. If there are still outstanding frames from the back, it is check at the end of every draw complete to switch back.

There are two independent threads. The first thread is the draw-write thread. These are protected by `encode_on_queue_semaphore` which ensures that each **draw-encode** and **write** event (that are dependent on and modify the `reference_counter` and `current_buffer`) happen independently and sequentially. The second thread is the draw-complete thread; protected by `index_swap_semaphore`. This contains **draw-complete**, and **swap-complete** events (that are dependent on and modify `reference_counter` and `active_buffer`). Something important to note is that **draw-complete** events happen in the same order of that they were encoded by the **draw-encode** events. Lastly, there are **swap-encode** events, which may happen on any thread and is not dependent on any reference variables. Write are protected by `complete_swap_semaphore`.

A **draw-encode** event increments the `current_buffer` reference counter by 1 and latches the `working_buffer` to the pointer to the buffer in `current_buffer`. This is so if `current_buffer` is changed while encoding, we have a consistent buffer bound. At the end of a draw encoding, the latch `working_buffer` is set to `nil`.

A **write-encode** event waits on the `complete_swap_semaphore` and protects itself by both `encode_draw_semaphore` and `index_swap_semaphore` so that no events can modify any of the reference variables. It sets `current_buffer` to `BackBuffer`. If there are any in-flight frames on the front buffer, set phase to `EncodeBack_drawFront`, if there are not, set phase `EncodeBack_drawBack`, set `active_buffer` to `BackBuffer` and initiate a **swap-encode** event. 

A **swap-complete** event protects itself by both `encode_draw_semaphore` and `index_swap_semaphore` so that no events can modify any of the reference variables. It sets `current_buffer` to `FrontBuffer`. If there are any in-flight frames on the back buffer, set phase to `EncodeFront_drawBack`, if there are not, set phase `WriteComplete_EncodeFront_drawFront`, set `active_buffer` to `FrontBuffer` and signal `complete_swap_semaphore`.

A **draw-complete** event is protected by the `index_swap_semaphore`. First it decrements the `active_buffer` reference counter. It then handles any phase changes which happen when the `active_buffer` reference counter is 0, and the phase is `EncodeBack_drawFront` or `EncodeFront_drawBack`. In the first case, `active_buffer` is set to the `BackBuffer`, the phase is set to `EncodeBack_drawBack` and a swap is initiated. In the second case, `active_buffer` is set to `FrontBuffer`, the phase is set to `EncodeFront_drawFront` and the `complete_swap_semaphore` is signaled.

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
