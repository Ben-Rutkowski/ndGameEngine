#import "DrawRoutineTemplates.h"
#include <Metal/Metal.h>

// ================ Draw Routine Template ================
@implementation DrawRoutineTemplate
{
    id<MTLDevice>       _device;
    id<MTLCommandQueue> _command_queue;
    
    NSMutableArray<id<BufferProtocol>>* _dynamic_buffers;
    NSMutableArray<id<BufferProtocol>>* _static_buffers;
    NSUInteger _current_buffer;
}


- (instancetype) initWithDevice:(nonnull id<MTLDevice>)device
                   commandQueue:(id<MTLCommandQueue>)command_queue
         numberOfDynamicBuffers:(NSUInteger)dynamic_buffer_count
          numberOfStaticBuffers:(NSUInteger)static_buffer_count
{
    self = [super init];
    if (self) {
        _device        = device;
        _command_queue = command_queue;
        _dynamic_buffers = [NSMutableArray arrayWithCapacity:dynamic_buffer_count];
        NullBuffer* null_buffer = [NullBuffer new];
        for (int i=0; i<dynamic_buffer_count; i++) {
            [_dynamic_buffers addObject:null_buffer];
        }

        _static_buffers = [NSMutableArray arrayWithCapacity:static_buffer_count];
        for (int i=0; i<static_buffer_count; i++) {
            [_static_buffers addObject:null_buffer];
        }
    }
    return self;
}


// ==== Resources ====
- (void)createBufferWithVertexSize:(NSUInteger)vertex_size 
                       vertexCount:(NSUInteger)vertex_count
{
    _dynamic_buffers[_current_buffer] = [[DynamicBuffer alloc] 
                                 initWithDevice:_device
                                 blitCommandQueue:_command_queue
                                 vertexSize:vertex_size
                                 vertexCount:vertex_count];
}

- (void) bindBuffer:(NSUInteger)index {
    _current_buffer = index;
}

- (id<BufferProtocol>) bufferAt:(NSUInteger)index {
    return _dynamic_buffers[index];
}

- (id<MTLBuffer>) writeBufferOpen {
    return [_dynamic_buffers[_current_buffer] writeOpen];
}

- (void) writeBufferClose {
    [_dynamic_buffers[_current_buffer] writeClose];
}


// ==== Draw ====
- (void) predrawOpenInBuffers {
    for (int i=0; i<_dynamic_buffers.count; i++) {
        [_dynamic_buffers[i] predrawOpen];
    }
}

- (void) predrawCloseInBuffers {
    for (int i=0; i<_dynamic_buffers.count; i++) {
        [_dynamic_buffers[i] predrawClose];
    }
}

- (void) drawCompletedInBuffers {
    for (int i=0; i<_dynamic_buffers.count; i++) {
        [_dynamic_buffers[i] drawCompleted];
    }
}


// ==== Debug ====
- (void) debugBuffer:(NSUInteger)vertex_count {
    [_dynamic_buffers[_current_buffer] debug:vertex_count];
}

@end
