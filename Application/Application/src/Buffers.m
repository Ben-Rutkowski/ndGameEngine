/*
    Draw   : (CPU) predraw stage -> (GPU) draw stage
    Resize : (CPU) presize stage -> (GPU) copy stage -> (CPU) swap stage
 
    We can read and write and resize the buffer.
 
    When reading:
        - Get current buffer index
        - Get current buffer vertex count
        - Increment active buffer reference count
    
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
    
    id<MTLBuffer> _buffer[2];
    NSUInteger    _vertex_count[2];
    NSUInteger    _refernce_count[2];
    
    NSUInteger    _vertex_size; // IMPLEMENT!!!!!
    
    NSUInteger _current_index; // The index of the newsest buffer
    NSUInteger _active_index;  // The index of the buffer with the next draw command
    
    BOOL isCurrentlySwapping;  // This is true when there are still draw
                               // commands active for the old buffer
                               // When true decrement active buffer first
                               // and once empty, delete old buffer, and swap active
    
    dispatch_semaphore_t _use_semaphore;
    dispatch_semaphore_t _complete_swap_semaphore;
    
    dispatch_semaphore_t _read_semaphore;
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
        
        _use_semaphore  = dispatch_semaphore_create(1);
        _read_semaphore = dispatch_semaphore_create(1);
        _complete_swap_semaphore = dispatch_semaphore_create(1);
    }
    
    return self;
}

- (NSUInteger) getVertexCount {
    return _vertex_count[_current_index];
}

- (void) completeSwap {
    if (isCurrentlySwapping && _refernce_count[_active_index] == 0) {
        NSLog(@"   === Swapping === : %lu", _current_index);
        isCurrentlySwapping = NO;
        _active_index       = _current_index;
        
        dispatch_semaphore_signal(_complete_swap_semaphore);
    }
}


// === Write ===
- (id<MTLBuffer>) writeOpen {
    NSLog(@"-- Write Buffer Open --");
    dispatch_semaphore_wait(_complete_swap_semaphore, DISPATCH_TIME_FOREVER);
    isCurrentlySwapping = YES;
    return _buffer[(_current_index+1)%2];
}

- (void) writeCloseInCommandBuffer:(nonnull id<MTLCommandBuffer>)command_buffer {
    NSUInteger next_index = (_current_index+1)%2;
    NSLog(@"-- Write Buffer Close -- : Current Buffer %lu to %lu of size %lu", next_index, _current_index, _vertex_count[next_index]);
    
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
    
    dispatch_semaphore_wait(_use_semaphore, DISPATCH_TIME_FOREVER);
    _current_index = (_current_index+1)%2;
    dispatch_semaphore_signal(_use_semaphore);
}


// ==== Draw ====
- (void) beginPredrawStage {
    dispatch_semaphore_wait(_use_semaphore, DISPATCH_TIME_FOREVER);
    _refernce_count[_current_index] += 1;
    NSLog(@"    Flying - Buffer 0: %lu, Buffer 1: %lu", _refernce_count[0], _refernce_count[1]);
}

- (void) endPredrawStage {
    dispatch_semaphore_signal(_use_semaphore);
}

- (void) endDrawStage {
    dispatch_semaphore_wait(_use_semaphore, DISPATCH_TIME_FOREVER);
    _refernce_count[_active_index] -= 1;
    NSLog(@"    Flying - Buffer 0: %lu, Buffer 1: %lu", _refernce_count[0], _refernce_count[1]);
    
    [self completeSwap];
    dispatch_semaphore_signal(_use_semaphore);
}

- (id<MTLBuffer>) drawTap {
    return _buffer[_current_index];
}


// ==== Resize ====
- (void) expandToDataSize:(NSUInteger)data_size
           andVertexCount:(NSUInteger)vertex_count
               withDevice:(nonnull id<MTLDevice>)device
           inCommandQueue:(nonnull id<MTLCommandQueue>)command_queue
{
    
//    dispatch_semaphore_wait(_complete_swap_semaphore, DISPATCH_TIME_FOREVER);
//    NSLog(@"wait : write full -- expandToDataSize");
//    dispatch_semaphore_wait(_write_semaphore, DISPATCH_TIME_FOREVER);
//    NSLog(@"wait : write half -- expandToDataSize");
//    NSUInteger next_index = (_current_index+1)%2;
//
//    _vertex_count[next_index] = vertex_count;
//    _buffer[next_index]       = [device newBufferWithLength:data_size
//                                                    options:_buffer[next_index].resourceOptions];
//
//    @autoreleasepool {
//        id<MTLCommandBuffer>      command_buffer = [command_queue commandBuffer];
//        id<MTLBlitCommandEncoder> blit_command   = [command_buffer blitCommandEncoder];
//        [blit_command copyFromBuffer:_buffer[_current_index]
//                        sourceOffset:0
//                            toBuffer:_buffer[next_index]
//                   destinationOffset:0
//                                size:_buffer[_current_index].length];
//        [blit_command endEncoding];
//
//        __block ResizableBuffer* block_buffer_self = self;
//        [command_buffer addCompletedHandler:^(id<MTLCommandBuffer> nonnull) {
//            [block_buffer_self initialSwapStage];
//        }];
//
//        [command_buffer commit];
//    }
}

- (void) initialSwapStage {
//    dispatch_semaphore_wait(_read_semaphore, DISPATCH_TIME_FOREVER);
//    NSLog(@"wait : read -- initialSwap");
//    isClearingOldBuffer = YES;
//    _current_index      = (_current_index+1)%2;
//    dispatch_semaphore_signal(_read_semaphore);
//    NSLog(@"signal : read -- initialSwap");
//    dispatch_semaphore_signal(_write_semaphore);
//    NSLog(@"signal : write half -- initialSwap");
}


// ==== Debug ====
- (void) debug:(NSUInteger)index {
//    NSLog(@"Buffer %lu : %@", index, _buffer[index]);
    NSLog(@"Buffer size : %lu", _buffer[index].length);
    NSLog(@"Buffer vertex count : %lu", _vertex_count[index]);
    
    float* vert = _buffer[index].contents;
    
    for (int i=0; i<12; i++) {
        NSLog(@"%f", vert[i]);
    }
}

- (void)debug {
}

@end
