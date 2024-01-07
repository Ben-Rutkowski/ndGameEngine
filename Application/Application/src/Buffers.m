/*
    The Phases of writing:
        - Write to the back buffer
        - When finished, SWAP (front to back) start encoding from the back buffer
        - When front buffer is finished being used, copy back to front
        - At the end of the copy, start encoding from the front again
        -
*/

#import "Buffers.h"

typedef enum SwapPhase {
    HasNotWriten                = 0,
    HasWritten_DrawingFromFront = 1,
    HasWritten_DrawingFromBack  = 2
} SwapPhase;

typedef enum BufferPosition {
    FrontBuffer = 0,
    BackBuffer  = 1
} BufferPosition;

@implementation DynamicBuffer
{
    id<MTLDevice>       _device;
    id<MTLCommandQueue> _command_queue;
    
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
    SwapPhase _swap_phase;
    
    dispatch_semaphore_t _index_swap_semaphore;
    dispatch_semaphore_t _encode_on_queue_semaphore;
    dispatch_semaphore_t _complete_swap_semaphore;
}


// ==== Configure ====
- (nonnull instancetype) initWithDevice:(nonnull id<MTLDevice>)device
                       blitCommandQueue:(nonnull id<MTLCommandQueue>)command_queue
                             vertexSize:(NSUInteger)vertex_size
                            vertexCount:(NSUInteger)vertex_count
                         andStorageMode:(MTLResourceOptions)storage_mode
{
    self = [super init];
    if (self) {
        _device        = device;
        _command_queue = command_queue;
        
        _current_index = 0;
        _active_index  = 0;
        _refernce_count[0] = 0;
        _refernce_count[1] = 0;
        _vertex_count[0] = vertex_count;
        _vertex_count[1] = vertex_count;
        _vertex_size = vertex_size;
        
        _buffer[0] = [_device newBufferWithLength:vertex_size*vertex_count
                                          options:storage_mode];
        _buffer[1] = [_device newBufferWithLength:vertex_size*vertex_count
                                          options:storage_mode];
            
        _swap_phase = HasNotWriten;
        _index_swap_semaphore      = dispatch_semaphore_create(1);
        _encode_on_queue_semaphore = dispatch_semaphore_create(1);
        _complete_swap_semaphore   = dispatch_semaphore_create(1);
        
        isCurrentlySwapping = NO;
    }
    
    return self;
}

- (NSUInteger) getVertexCount {
    return _vertex_count[_current_index];
}


// ==== Swap ====
- (void) checkSwapConditions {
    switch (_swap_phase) {
        case HasWritten_DrawingFromFront: {
            NSLog(@"(Checking Front)");
            if (_refernce_count[FrontBuffer] == 0) {
                _active_index = BackBuffer;
                _swap_phase   = HasWritten_DrawingFromBack;
                [self copyBackToFront];
            }
            
            break;
        }
            
        case HasWritten_DrawingFromBack: {
            if (_refernce_count[BackBuffer] == 0) {
                NSLog(@"(Checking Back)");
                _active_index = FrontBuffer;
                _swap_phase   = HasNotWriten;
                
                NSLog(@"=== Swap Completed ===");
                NSLog(@"current: %lu, active: %lu", _current_index, _active_index);
                dispatch_semaphore_signal(_complete_swap_semaphore);
            }
            break;
        }
        default: break;
    }
}

- (void) swapBackToFront {
    dispatch_semaphore_wait(_index_swap_semaphore, DISPATCH_TIME_FOREVER);
    NSLog(@"-- Swapping Index Back to Front -- ");
    _current_index = FrontBuffer;
    _swap_phase    = HasWritten_DrawingFromBack;
//    [self checkSwapConditions];
    dispatch_semaphore_signal(_index_swap_semaphore);
}

- (void) copyBackToFront {
    dispatch_semaphore_wait(_encode_on_queue_semaphore, DISPATCH_TIME_FOREVER);
    NSLog(@"-- Encode Copy Back to Front --");
    @autoreleasepool {
        id<MTLCommandBuffer>      command_buffer = [_command_queue commandBuffer];
        id<MTLBlitCommandEncoder> blit_encoder   = [command_buffer blitCommandEncoder];
        
        [blit_encoder copyFromBuffer:_buffer[BackBuffer]
                        sourceOffset:0
                            toBuffer:_buffer[FrontBuffer]
                   destinationOffset:0
                                size:_vertex_size*_vertex_count[BackBuffer]];
        
        [blit_encoder endEncoding];
        
        __block DynamicBuffer* block_buffer = self;
        [command_buffer addCompletedHandler:^(id<MTLCommandBuffer> nonnull) {
            [block_buffer swapBackToFront];
        }];
        
        [command_buffer commit];
    }
    dispatch_semaphore_signal(_encode_on_queue_semaphore);
}


// ==== Draw ====
- (void) predrawOpen {
    dispatch_semaphore_wait(_encode_on_queue_semaphore, DISPATCH_TIME_FOREVER);
    NSLog(@"-- Predraw Open --");
    _working_index = _current_index;
    _refernce_count[_working_index] += 1;
}

- (void) predrawClose {
    NSLog(@"  New Frames in flight, -->[ %lu, %lu ]", _refernce_count[0], _refernce_count[1]);
    NSLog(@"-- Predraw Close --");
    dispatch_semaphore_signal(_encode_on_queue_semaphore);
}

- (void) drawCompleted {
    dispatch_semaphore_wait(_index_swap_semaphore, DISPATCH_TIME_FOREVER);
    _refernce_count[_active_index] -= 1;
    NSLog(@"-- Draw Complete --");
    NSLog(@"  Remaining Frames in flight, [ %lu, %lu ]<--", _refernce_count[0], _refernce_count[1]);
    [self checkSwapConditions];
    dispatch_semaphore_signal(_index_swap_semaphore);
}

- (id<MTLBuffer>) drawTap {
    return _buffer[_working_index];
}


// ==== Write ====
- (id<MTLBuffer>) writeOpen {
    dispatch_semaphore_wait(_complete_swap_semaphore, DISPATCH_TIME_FOREVER);
    NSLog(@"-- Write Open --");
    isCurrentlySwapping = YES;
    return _buffer[(_current_index+1)%2];
}

- (void) writeClose {
    dispatch_semaphore_wait(_index_swap_semaphore, DISPATCH_TIME_FOREVER);
    dispatch_semaphore_wait(_encode_on_queue_semaphore, DISPATCH_TIME_FOREVER);
    _swap_phase    = HasWritten_DrawingFromFront;
    _current_index = BackBuffer;
    if (_refernce_count[FrontBuffer] == 0) {
        _active_index = BackBuffer;
    }
    NSLog(@"-- Write Close -- (Drawing From Back)");
    dispatch_semaphore_signal(_encode_on_queue_semaphore);
    dispatch_semaphore_signal(_index_swap_semaphore);
}


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
