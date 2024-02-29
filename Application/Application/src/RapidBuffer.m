#import "Buffers.h"
#include <objc/NSObjCRuntime.h>

@implementation RapidBuffer
{
    id<MTLDevice>       _device;
    id<MTLCommandQueue> _command_queue;

//    --- Buffers Data ---
    id<MTLBuffer> _buffers[3];
    id<MTLBuffer> _working_buffer;

    NSUInteger _vertex_count;
    NSUInteger _vertex_size;

    NSUInteger _next_to_draw;  // The next buffer to be drawn
    NSUInteger _next_to_write; // The next available buffer to be written

//    --- Thread Controllers ---
    dispatch_semaphore_t _in_flight_semaphore;

}


// ==== Configure ==== 
- (instancetype) initWithDevice:(nonnull id<MTLDevice>)device
               blitCommandQueue:(nonnull id<MTLCommandQueue>)command_queue
                     vertexSize:(NSUInteger)vertex_size
                    vertexCount:(NSUInteger)vertex_count
{
    self = [super init];
    if (self) {
        _device        = device;
        _command_queue = command_queue;

        _vertex_size  = vertex_size;
        _vertex_count = vertex_count;

        _next_to_draw  = 0;
        _next_to_write = 0;

        _buffers[0] = [_device newBufferWithLength:vertex_size*vertex_count 
                                           options:MTLResourceStorageModeShared];
        _buffers[1] = [_device newBufferWithLength:vertex_size*vertex_count 
                                           options:MTLResourceStorageModeShared];
        _buffers[2] = [_device newBufferWithLength:vertex_size*vertex_count 
                                           options:MTLResourceStorageModeShared];

        _in_flight_semaphore   = dispatch_semaphore_create(3);
    }

    return self;
}

- (NSUInteger) getVertexCount {
    return  _vertex_count;
}


// ==== Draw ====
- (id<MTLBuffer>) drawTap {
    return _buffers[_next_to_draw];
}

- (id<MTLBuffer>) drawRelay {
    return nil;
}

- (void) predrawOpen {
}

- (void) predrawClose {
    _next_to_draw = (_next_to_draw+1)%3;
}

- (void) drawCompleted {
    dispatch_semaphore_signal(_in_flight_semaphore);
}


// ==== Write ====
- (id<MTLBuffer>) writeOpen {
    dispatch_semaphore_wait(_in_flight_semaphore, DISPATCH_TIME_FOREVER);
    return _buffers[_next_to_write];
}

- (void) writeClose {
    _next_to_write = (_next_to_write+1)%3;
}


// ==== Debug ====
- (void) debug:(NSUInteger)num_vertices {
}

@end
