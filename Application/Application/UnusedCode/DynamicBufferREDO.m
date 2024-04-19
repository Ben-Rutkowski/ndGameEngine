#import "Buffers.h"

typedef enum BufferPosition {
    FrontBuffer = 0,
    BackBuffer  = 1
} BufferPosition;

@implementation DynamicBuffer
{
    id<MTLDevice>       _device;
    id<MTLCommandQueue> _command_queue;

    // --- Buffer Data ---
    id<MTLBuffer> _buffer[2];
    id<MTLBuffer> _working_buffer;
    NSUInteger    _vertex_count[2];
    NSUInteger    _refernce_count[2];

    NSUInteger _current_index;
    NSUInteger _active_index;

    NSUInteger _vertex_size;

    // --- Thread Controllers ---
    dispatch_semaphore_t _active_index_semaphore;
    dispatch_semaphore_t _current_index_semaphore;
    dispatch_semaphore_t _complete_swap_semaphore;
}


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
            
        _active_index_semaphore  = dispatch_semaphore_create(1);
        _current_index_semaphore = dispatch_semaphore_create(1);
        _complete_swap_semaphore = dispatch_semaphore_create(1);
    }
    
    return self;
}

- (NSUInteger) getVertexCount {
    return _vertex_count[_current_index];
}


// ================ Swap ================ 
- (void) swapBackToFront {
    dispatch_semaphore_wait(_current_index_semaphore, DISPATCH_TIME_FOREVER);
    dispatch_semaphore_wait(_active_index_semaphore, DISPATCH_TIME_FOREVER);
    _current_index = FrontBuffer;
    if (_active_index == BackBuffer &&
        _refernce_count[_active_index] == 0)
    {
        _active_index = FrontBuffer;
        dispatch_semaphore_signal(_complete_swap_semaphore);
    }

    dispatch_semaphore_signal(_current_index_semaphore);
    dispatch_semaphore_signal(_active_index_semaphore);
}

- (void) copyBackToFront {
    @autoreleasepool {
        id<MTLCommandBuffer> command_buffer    = [_command_queue commandBuffer];
        id<MTLBlitCommandEncoder> blit_encoder = [command_buffer blitCommandEncoder];

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


// ================ Draw ================
- (void) predrawOpen {
    dispatch_semaphore_wait(_current_index_semaphore, DISPATCH_TIME_FOREVER);
    _refernce_count[_current_index] += 1;
    _working_buffer = _buffer[_current_index];
    dispatch_semaphore_signal(_current_index_semaphore);
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
    dispatch_semaphore_wait(_active_index_semaphore, DISPATCH_TIME_FOREVER);
    _refernce_count[_active_index] -= 1;

    if (_current_index == BackBuffer  && 
        _active_index  == FrontBuffer && 
        _refernce_count[FrontBuffer] == 0)
    {
        _active_index = BackBuffer;
        dispatch_semaphore_wait(_current_index_semaphore, DISPATCH_TIME_FOREVER);
        [self copyBackToFront];
        dispatch_semaphore_signal(_current_index_semaphore);
    } 
    else if (_current_index == FrontBuffer &&
             _active_index  == BackBuffer  &&
             _refernce_count[BackBuffer] == 0) 
    {
        _active_index = FrontBuffer;
        dispatch_semaphore_signal(_complete_swap_semaphore);
    }
    dispatch_semaphore_signal(_active_index_semaphore);
}


// ================ Write ================
- (id<MTLBuffer>) writeOpen {
    dispatch_semaphore_wait(_complete_swap_semaphore, DISPATCH_TIME_FOREVER);
    return _buffer[BackBuffer];
}

- (void) writeClose {
    dispatch_semaphore_wait(_current_index_semaphore, DISPATCH_TIME_FOREVER);
    dispatch_semaphore_wait(_active_index_semaphore, DISPATCH_TIME_FOREVER);
    _current_index = BackBuffer;
    if (_refernce_count[FrontBuffer] == 0) {
        _active_index = BackBuffer;
        [self copyBackToFront];
    }
    dispatch_semaphore_signal(_current_index_semaphore);
    dispatch_semaphore_signal(_active_index_semaphore);
}

- (void) writeCloseWithRange:(NSRange)range {}


// ================ Debug ================
- (void) debug:(NSUInteger)num_vertices {
    NSLog(@"Current Index : %lu, Active Index : %lu", _current_index, _active_index);
    NSLog(@" ==== Buffer ZERO: ====");
    [self debugBuffer:0 numVertex:num_vertices];
}

- (void) debugBuffer:(NSUInteger)index numVertex:(NSUInteger)num_vertices {
    dispatch_semaphore_wait(_complete_swap_semaphore, DISPATCH_TIME_FOREVER);
    dispatch_semaphore_signal(_complete_swap_semaphore);

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
