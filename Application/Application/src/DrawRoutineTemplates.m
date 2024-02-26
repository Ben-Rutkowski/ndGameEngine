#import "DrawRoutineTemplates.h"

// ================ Draw Routine Template ================
@implementation DrawRoutineTemplate
{
    id<MTLDevice>       _device;
    id<MTLCommandQueue> _command_queue;
    
    NSMutableArray<id<BufferProtocol>>* _buffers;
    NSUInteger _current_buffer;
}


- (instancetype) initWithDevice:(nonnull id<MTLDevice>)device
                   commandQueue:(id<MTLCommandQueue>)command_queue
                numberOfBuffers:(NSUInteger)buffer_count
{
    self = [super init];
    if (self) {
        _device        = device;
        _command_queue = command_queue;
        _buffers = [NSMutableArray arrayWithCapacity:buffer_count];
        NullBuffer* null_buffer = [NullBuffer new];
        for (int i=0; i<buffer_count; i++) {
            [_buffers addObject:null_buffer];
        }
    }
    return self;
}


// ==== Resources ====
- (void)createBufferWithVertexSize:(NSUInteger)vertex_size 
                       vertexCount:(NSUInteger)vertex_count
                       storageMode:(MTLResourceOptions)storage_mode
{
    _buffers[_current_buffer] = [[DynamicBuffer alloc] 
                                 initWithDevice:_device
                                 blitCommandQueue:_command_queue
                                 vertexSize:vertex_size
                                 vertexCount:vertex_count
                                 andStorageMode:storage_mode];
//    [_buffers[_current_buffer]debug:1];
}

- (void) bindBuffer:(NSUInteger)index {
    _current_buffer = index;
}

- (id<BufferProtocol>) bufferAt:(NSUInteger)index {
    return _buffers[index];
}

- (id<MTLBuffer>) writeBufferOpen {
    return [_buffers[_current_buffer] writeOpen];
}

- (void) writeBufferClose {
    [_buffers[_current_buffer] writeClose];
}


// ==== Draw ====
- (void) predrawOpenInBuffers {
    for (int i=0; i<_buffers.count; i++) {
        [_buffers[i] predrawOpen];
    }
}

- (void) predrawCloseInBuffers {
    for (int i=0; i<_buffers.count; i++) {
        [_buffers[i] predrawClose];
    }
}

- (void) drawCompletedInBuffers {
    for (int i=0; i<_buffers.count; i++) {
        [_buffers[i] drawCompleted];
    }
}


// ==== Debug ====
- (void) debugBuffer:(NSUInteger)vertex_count {
    [_buffers[_current_buffer] debug:vertex_count];
}

@end
