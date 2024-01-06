/*
    We can read and write and resize the buffer.
 
    When reading:
        - Get working buffer index
        - Get working buffer vertex count
        - Increment working buffer reference count
    
    After drawing:
        - Decrement active buffer count
        - Check if active buffer is empty and
            complete swap if necessary.
 
    When writing:
        - Modify contents in
*/

#import "Buffers.h"

@implementation DynamicBuffer
{
    id<MTLDevice> _device;
    
//    --- Buffer Data ---
    id<MTLBuffer> _buffer[2];
    NSUInteger    _vertex_count[2];
    NSUInteger    _refernce_count[2];
    
    NSUInteger _current_index; // The index of the newsest buffer
    NSUInteger _active_index;  // The index of the buffer with the next draw command
    NSUInteger _working_index; // The index of the buffer being used to encode current draw
    
    NSUInteger _vertex_size;
    
//    --- Thread Controllers ---
    BOOL isCurrentlySwapping;  // This is true when there are still draw
                               // commands active for the old buffer
                               // When true decrement active buffer first
                               // and once empty, delete old buffer, and swap active
    
    dispatch_semaphore_t _index_swap_semaphore;
    dispatch_semaphore_t _draw_encode_semaphore;
    dispatch_semaphore_t _complete_swap_semaphore;
}


// ==== Configure ====
- (nonnull instancetype) initWithDevice:(nonnull id<MTLDevice>)device
                             vertexSize:(NSUInteger)vertex_size
                            vertexCount:(NSUInteger)vertex_count
                         andStorageMode:(MTLResourceOptions)storage_mode
{
    self = [super init];
    if (self) {
        _current_index = 0;
        _active_index  = 0;
        _refernce_count[0] = 0;
        _refernce_count[1] = 0;
        _vertex_count[0] = vertex_count;
        _vertex_count[1] = vertex_count;
        _vertex_size = vertex_size;
        
        _buffer[0] = [device newBufferWithLength:vertex_size*vertex_count
                                         options:storage_mode];
        _buffer[1] = [device newBufferWithLength:vertex_size*vertex_count
                                         options:storage_mode];
            
        isCurrentlySwapping = NO;
        _index_swap_semaphore    = dispatch_semaphore_create(1);
        _draw_encode_semaphore   = dispatch_semaphore_create(1);
        _complete_swap_semaphore = dispatch_semaphore_create(1);
    }
    
    return self;
}

- (NSUInteger) getVertexCount {
    return _vertex_count[_current_index];
}


// ==== Draw ====
- (void) predrawOpen {
    dispatch_semaphore_wait(_draw_encode_semaphore, DISPATCH_TIME_FOREVER);
    _working_index = _current_index;
    _refernce_count[_working_index] += 1;
}

- (void) predrawClose {
    dispatch_semaphore_signal(_draw_encode_semaphore);
}

- (void) drawCompleted {
    dispatch_semaphore_wait(_index_swap_semaphore, DISPATCH_TIME_FOREVER);
    _refernce_count[_active_index] -= 1;
    [self completeSwap];
    dispatch_semaphore_signal(_index_swap_semaphore);
}

- (id<MTLBuffer>) drawTap {
    return _buffer[_working_index];
}


// ==== Write ====
- (void) completeSwap {
    if (isCurrentlySwapping && _refernce_count[_active_index] == 0) {
        isCurrentlySwapping = NO;
        _active_index       = _current_index;
        
        dispatch_semaphore_signal(_complete_swap_semaphore);
    }
}

- (id<MTLBuffer>) writeOpen {
    dispatch_semaphore_wait(_complete_swap_semaphore, DISPATCH_TIME_FOREVER);
    isCurrentlySwapping = YES;
    return _buffer[(_current_index+1)%2];
}

- (void) writeCloseInBlitCommandBuffer:(nonnull id<MTLCommandBuffer>)command_buffer {
    NSUInteger next_index = (_current_index+1)%2;
    
    @autoreleasepool {
        id<MTLBlitCommandEncoder> blit_encoder = [command_buffer blitCommandEncoder];
        
        [blit_encoder copyFromBuffer:_buffer[next_index]
                        sourceOffset:0
                            toBuffer:_buffer[_current_index]
                   destinationOffset:0
                                size:_vertex_size*_vertex_count[next_index]];
        
        [blit_encoder endEncoding];
        
        __block DynamicBuffer* block_self = self;
        [command_buffer addCompletedHandler:^(id<MTLCommandBuffer> nonnull) {
            [block_self completeSwap];
        }];
    }
    
    dispatch_semaphore_wait(_index_swap_semaphore, DISPATCH_TIME_FOREVER);
    _current_index = (_current_index+1)%2;
    dispatch_semaphore_signal(_index_swap_semaphore);
}


// ==== Resize ====
//- (void) expandToDataSize:(NSUInteger)data_size
//           andVertexCount:(NSUInteger)vertex_count
//               withDevice:(nonnull id<MTLDevice>)device
//           inCommandQueue:(nonnull id<MTLCommandQueue>)command_queue
//{
//
//}
//
//- (void) initialSwapStage {
//    
//}


// ==== Debug ====
- (void) debug:(NSUInteger)num_vertices {
    NSLog(@"Current Index : %lu, Active Index : %lu, Working Index : %lu", _current_index, _active_index, _working_index);
    NSLog(@" ==== Buffer ZERO: ====");
    [self debugBuffer:0 numVertex:num_vertices];
    NSLog(@" ==== Buffer ONE: ====");
    [self debugBuffer:1 numVertex:num_vertices];
}

- (void) debugBuffer:(NSUInteger)index numVertex:(NSUInteger)num_vertices {
    NSLog(@"Buffer size : %lu", _buffer[index].length);
    NSLog(@"Buffer vertex count : %lu", _vertex_count[index]);
    
    float* vert = _buffer[index].contents;
    
    NSLog(@"Size of vertex : %lu", _vertex_size);
    
    int width = (int)_vertex_size/(int)4;
    for (int base=0; base<num_vertices; base++) {
        NSLog(@"--- Vertex %i ---", base);
        for (int i=0; i<width; i++) {
            NSLog(@"%f", vert[base*width + i]);
        }
    }
}

@end
