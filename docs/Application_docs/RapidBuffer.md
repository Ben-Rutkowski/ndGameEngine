# **Class** | Rapid Buffer
A buffer that is meant to be rewritten every frame.
```
@interface RapidBuffer : NSObject<BufferProtocol>
```

### Description
Keeps a revolving set of three `id<MTLBuffer>` (the maximum number of in flight frames that Metal uses). There are two counters that keep track of the `_next_to_draw` and `_next_to_write` indices. 

Upon each write, the `_next_to_write` index is incremented modulus 3. Upon each draw encoding completion `_next_to_draw` is incremented modulus 3. 

Writes and draws are protected by the `_in_flight_semaphore` of size 3 that permits at most three buffers to be written and drawn.