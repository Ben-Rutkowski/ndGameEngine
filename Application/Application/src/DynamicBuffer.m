#import "Buffers.h"

typedef enum SwapPhase {
    WriteComplete_EncodeFront_DrawFront = 0,
    EncodingBack_DrawingFront           = 1,
    EncodingFront_DrawingBack           = 2,
    EncodingBack_DrawingBack            = 3
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
    id<MTLBuffer> _working_buffer;
    NSUInteger    _vertex_count[2];
    NSUInteger    _refernce_count[2];
    
    NSUInteger _current_index; // The index of the newest buffer
    NSUInteger _active_index;  // The index of the buffer with the next draw command
    
    NSUInteger _vertex_size;
    
//    --- Thread Controllers ---
    SwapPhase _swap_phase;
    
    dispatch_semaphore_t _index_swap_semaphore;
    dispatch_semaphore_t _encode_on_queue_semaphore;
    dispatch_semaphore_t _complete_swap_semaphore;
}


// ==== Configure ====
// - (nonnull instancetype) initWithDevice:(nonnull id<MTLDevice>)device
//                        blitCommandQueue:(nonnull id<MTLCommandQueue>)command_queue
//                              vertexSize:(NSUInteger)vertex_size
//                             vertexCount:(NSUInteger)vertex_count
//                          andStorageMode:(MTLResourceOptions)storage_mode
// {
//     self = [super init];
//     if (self) {
//         _device        = device;
//         _command_queue = command_queue;
//         
//         _current_index = FrontBuffer;
//         _active_index  = FrontBuffer;
//         _refernce_count[FrontBuffer] = 0;
//         _refernce_count[BackBuffer]  = 0;
//         _vertex_count[FrontBuffer] = vertex_count;
//         _vertex_count[BackBuffer]  = vertex_count;
//         _vertex_size = vertex_size;
//         
//         _buffer[FrontBuffer] = [_device newBufferWithLength:vertex_size*vertex_count
//                                           options:MTLResourceStorageModePrivate];
//         _buffer[BackBuffer]  = [_device newBufferWithLength:vertex_size*vertex_count
//                                            options:MTLResourceStorageModeShared];
//             
//         _swap_phase = WriteComplete_EncodeFront_DrawFront;
//         _index_swap_semaphore      = dispatch_semaphore_create(1);
//         _encode_on_queue_semaphore = dispatch_semaphore_create(1);
//         _complete_swap_semaphore   = dispatch_semaphore_create(1);
//     }
//     
//     return self;
// }

- (nonnull instancetype) initWithDevice:(nonnull id<MTLDevice>)device
                       blitCommandQueue:(nonnull id<MTLCommandQueue>)command_queue
                             vertexSize:(NSUInteger)vertex_size
                            vertexCount:(NSUInteger)vertex_count
{
    self = [super init];
    if (self) {
        _device        = device;
        _command_queue = command_queue;
        
        _current_index = FrontBuffer;
        _active_index  = FrontBuffer;
        _refernce_count[FrontBuffer] = 0;
        _refernce_count[BackBuffer]  = 0;
        _vertex_count[FrontBuffer] = vertex_count;
        _vertex_count[BackBuffer]  = vertex_count;
        _vertex_size = vertex_size;
        
        _buffer[FrontBuffer] = [_device newBufferWithLength:vertex_size*vertex_count
                                          options:MTLResourceStorageModePrivate];
        _buffer[BackBuffer]  = [_device newBufferWithLength:vertex_size*vertex_count
                                           options:MTLResourceStorageModeShared];
            
        _swap_phase = WriteComplete_EncodeFront_DrawFront;
        _index_swap_semaphore      = dispatch_semaphore_create(1);
        _encode_on_queue_semaphore = dispatch_semaphore_create(1);
        _complete_swap_semaphore   = dispatch_semaphore_create(1);
    }
    
    return self;
}

- (NSUInteger) getVertexCount {
    return _vertex_count[_current_index];
}


// ==== Swap ====
- (void) swapBackToFront {
    dispatch_semaphore_wait(_encode_on_queue_semaphore, DISPATCH_TIME_FOREVER);
    dispatch_semaphore_wait(_index_swap_semaphore, DISPATCH_TIME_FOREVER);
    _current_index = FrontBuffer;
    if (_refernce_count[BackBuffer] == 0) {
        _swap_phase   = WriteComplete_EncodeFront_DrawFront;
        _active_index = FrontBuffer;
        dispatch_semaphore_signal(_complete_swap_semaphore);
    } else {
        _swap_phase = EncodingFront_DrawingBack;
    }
    dispatch_semaphore_signal(_index_swap_semaphore);
    dispatch_semaphore_signal(_encode_on_queue_semaphore);
}

- (void) copyBackToFront {
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
}


// ==== Draw ====
- (void) predrawOpen {
    dispatch_semaphore_wait(_encode_on_queue_semaphore, DISPATCH_TIME_FOREVER);
    _refernce_count[_current_index] += 1;
    _working_buffer = _buffer[_current_index];
    dispatch_semaphore_signal(_encode_on_queue_semaphore);
}

- (id<MTLBuffer>) drawTap {
    return _working_buffer;
}

- (id<MTLBuffer>) drawRelay {
    return _buffer[FrontBuffer];
}

- (void) predrawClose {
    _working_buffer = nil;
}

- (void) drawCompleted {
    dispatch_semaphore_wait(_index_swap_semaphore, DISPATCH_TIME_FOREVER);
    _refernce_count[_active_index] -= 1;
    switch (_swap_phase) {
        case EncodingBack_DrawingFront: {
            if (_refernce_count[FrontBuffer] == 0) {
                _swap_phase   = EncodingBack_DrawingBack;
                _active_index = BackBuffer;
                dispatch_semaphore_signal(_index_swap_semaphore);
                [self copyBackToFront];
                return;
            }
            break;
        } case EncodingFront_DrawingBack: {
            if (_refernce_count[BackBuffer] == 0) {
                _swap_phase   = WriteComplete_EncodeFront_DrawFront;
                _active_index = FrontBuffer;
                dispatch_semaphore_signal(_complete_swap_semaphore);
            }
            break;
        }
        default: break;
    }
    dispatch_semaphore_signal(_index_swap_semaphore);
}


// ==== Write ====
- (id<MTLBuffer>) writeOpen {
    dispatch_semaphore_wait(_complete_swap_semaphore, DISPATCH_TIME_FOREVER);
    return _buffer[BackBuffer];
}

- (void) writeClose {
    dispatch_semaphore_wait(_encode_on_queue_semaphore, DISPATCH_TIME_FOREVER);
    dispatch_semaphore_wait(_index_swap_semaphore, DISPATCH_TIME_FOREVER);
    _current_index = BackBuffer;
    if (_refernce_count[FrontBuffer] == 0) {
        _swap_phase   = EncodingBack_DrawingBack;
        _active_index = BackBuffer;
        dispatch_semaphore_signal(_index_swap_semaphore);
        dispatch_semaphore_signal(_encode_on_queue_semaphore);
        [self copyBackToFront];
        return;
    } else {
        _swap_phase = EncodingBack_DrawingFront;
    }
    dispatch_semaphore_signal(_index_swap_semaphore);
    dispatch_semaphore_signal(_encode_on_queue_semaphore);
}


// ==== Debug ====
- (void) debug:(NSUInteger)num_vertices {
    NSLog(@"Current Index : %lu, Active Index : %lu", _current_index, _active_index);
    NSLog(@" ==== Buffer ZERO: ====");
    [self debugBuffer:0 numVertex:num_vertices];
//    NSLog(@" ==== Buffer ONE: ====");
//    [self debugBuffer:1 numVertex:num_vertices];
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


// ================ Null Dynamic Buffer ================
@implementation NullBuffer
- (NSUInteger) getVertexCount { return 0; }
- (void) predrawOpen {}
- (id<MTLBuffer>) drawRelay { return nil; }
- (id<MTLBuffer>) drawTap { return nil; }
- (void) predrawClose {}
- (void) drawCompleted {}
- (void) writeClose {}
- (id<MTLBuffer>) writeOpen { return nil; }
- (void) debug:(NSUInteger)num_vertices {}

@end
