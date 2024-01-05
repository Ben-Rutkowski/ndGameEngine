/*
    Draw   : (CPU) predraw stage -> (GPU) draw stage
    Resize : (CPU) presize stage -> (GPU) copy stage -> (CPU) swap stage
*/

#import "Buffers.h"

@implementation ResizableBuffer
{
    id<MTLBuffer> _buffer[2];
    NSUInteger    _vertex_count[2];
    NSUInteger    _refernce_count[2];
    
    NSUInteger _current_index; // The index of the newsest buffer
    NSUInteger _active_index;  // The index of the buffer with the next draw command
    
    BOOL isEmptyingOldBuffer;  // This is true when there are still draw
                               // commands active for the old buffer
                               // When true decrement active buffer first
                               // and once empty, delete old buffer, and swap active
    
    dispatch_semaphore_t _resize_create_semaphore;
    dispatch_semaphore_t _resize_full_semaphore;
    dispatch_semaphore_t _reference_semaphore;
}


// ==== Configure ====
- (nonnull instancetype) initWithDevice:(nonnull id<MTLDevice>)device
                               dataSize:(NSUInteger)data_size
                            vertexCount:(NSUInteger)vertex_count
                         andStorageMode:(MTLResourceOptions)storage_mode
{
    self = [super init];
    if (self) {
        _current_index = 0;
        _active_index  = 0;
        _buffer[_current_index] = [device newBufferWithLength:data_size
                                                             options:storage_mode];
        _vertex_count[_current_index] = vertex_count;
        _refernce_count[0] = 0;
        _refernce_count[1] = 0;
        isEmptyingOldBuffer = NO;
        
        _resize_create_semaphore = dispatch_semaphore_create(1);
        _resize_full_semaphore   = dispatch_semaphore_create(1);
        _reference_semaphore     = dispatch_semaphore_create(1);
    }
    
    return self;
}


// ==== Size ====
- (NSUInteger) getVertexCount {
    return _vertex_count[_current_index];
}

- (void) expandToDataSize:(NSUInteger)data_size
           andVertexCount:(NSUInteger)vertex_count
               withDevice:(nonnull id<MTLDevice>)device
           inCommandQueue:(nonnull id<MTLCommandQueue>)command_queue
{
    dispatch_semaphore_wait(_resize_full_semaphore, DISPATCH_TIME_FOREVER);
    NSLog(@"wait : resize full -- expandToDataSize");
    dispatch_semaphore_wait(_resize_create_semaphore, DISPATCH_TIME_FOREVER);
    NSLog(@"wait : resize create -- expandToDataSize");
    NSUInteger next_index = (_current_index+1)%2;
    
    _vertex_count[next_index] = vertex_count;
    _buffer[next_index]       = [device newBufferWithLength:data_size
                                                    options:_buffer[next_index].resourceOptions];
    
    @autoreleasepool {
        id<MTLCommandBuffer>      command_buffer = [command_queue commandBuffer];
        id<MTLBlitCommandEncoder> blit_command   = [command_buffer blitCommandEncoder];
        [blit_command copyFromBuffer:_buffer[_current_index]
                        sourceOffset:0
                            toBuffer:_buffer[next_index]
                   destinationOffset:0
                                size:_buffer[_current_index].length];
        [blit_command endEncoding];
        
        __block ResizableBuffer* block_buffer_self = self;
        [command_buffer addCompletedHandler:^(id<MTLCommandBuffer> nonnull) {
            [block_buffer_self initialSwapStage];
        }];
        
        [command_buffer commit];
    }
}

- (void) initialSwapStage {
    dispatch_semaphore_wait(_reference_semaphore, DISPATCH_TIME_FOREVER);
    NSLog(@"wait : reference -- initialSwap");
    isEmptyingOldBuffer = YES;
    _current_index      = (_current_index+1)%2;
    dispatch_semaphore_signal(_reference_semaphore);
    NSLog(@"signal : reference -- initialSwap");
    dispatch_semaphore_signal(_resize_create_semaphore);
    NSLog(@"signal : resize create -- initialSwap");
}


// ==== Draw ====
- (void) beginPredrawStage {
    dispatch_semaphore_wait(_reference_semaphore, DISPATCH_TIME_FOREVER);
    NSLog(@"wait : reference -- beginPredrawStage");
    _refernce_count[_active_index] += 1;
}

- (void) endPredrawStage {
    NSLog(@"     Flying; %lu frames on buffer : %lu", _refernce_count[_active_index], _active_index);
    dispatch_semaphore_signal(_reference_semaphore);
    NSLog(@"signal : reference -- endPredrawStage");
}

- (void) beginDrawStage {}

- (void) endDrawStage {
    dispatch_semaphore_wait(_reference_semaphore, DISPATCH_TIME_FOREVER);
    NSLog(@"wait : reference -- endDrawStage");
    _refernce_count[_active_index] -= 1;
    NSLog(@"     Finished; %lu frames remain on buffer : %lu", _refernce_count[_active_index], _active_index);
    if (isEmptyingOldBuffer && _refernce_count[_active_index] == 0) {
        isEmptyingOldBuffer = NO;
        NSLog(@"     Errasing Old Buffer -- endDrawStage");
        [_buffer[_active_index] setPurgeableState:MTLPurgeableStateEmpty];
        [_buffer[_active_index] release];
        _active_index = _current_index;
        dispatch_semaphore_signal(_resize_full_semaphore);
        NSLog(@"signal : resize full -- endDrawStage");
    }
    dispatch_semaphore_signal(_reference_semaphore);
    NSLog(@"signal : reference -- endDrawStage");
}

- (id<MTLBuffer>) drawTap {
    return _buffer[_current_index];
}


// === Write ===
- (id<MTLBuffer>) editTap {
    return _buffer[_current_index];
}

- (void) editUntap {
    
}


// ==== Debug ====
- (void) debug {
    NSLog(@"Buffer size : %lu", _buffer[_current_index].length);
    NSLog(@"Buffer vertex count : %lu", _vertex_count[_current_index]);
    
    float* vert = _buffer[_current_index].contents;
    
    for (int i=0; i<4; i++) {
        NSLog(@"%f", vert[i]);
    }
}

@end
