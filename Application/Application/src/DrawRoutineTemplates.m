#import "DrawRoutineTemplates.h"

// ================ Draw Routine Template ================
@implementation DrawRoutineTemplate
{
    id<MTLDevice>       _device;
    id<MTLCommandQueue> _internal_blit_command_queue;
    
    NSMutableArray<DynamicBuffer*>* _buffers;
    NSUInteger _current_buffer;
}


- (instancetype) initWithDevice:(nonnull id<MTLDevice>)device
                numberOfBuffers:(NSUInteger)buffer_count
{
    self = [super init];
    if (self) {
        _device                      = device;
        _internal_blit_command_queue = [_device newCommandQueue];
        _buffers = [NSMutableArray arrayWithCapacity:buffer_count];
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
                                 vertexSize:vertex_size
                                 vertexCount:vertex_count
                                 andStorageMode:storage_mode];
//    [_buffers[_current_buffer]debug:1];
}

- (void) bindBuffer:(NSUInteger)index {
    NSLog(@"Bind Buffer : %lu", index);
    _current_buffer = index;
}

- (DynamicBuffer*) bufferAt:(NSUInteger)index {
    return _buffers[index];
}

- (id<MTLBuffer>) writeBufferOpen {
    return [_buffers[_current_buffer] writeOpen];
}

- (void) writeBufferClose {
    @autoreleasepool {
        id<MTLCommandBuffer> command_buffer = [_internal_blit_command_queue commandBuffer];
        [_buffers[_current_buffer] writeCloseInBlitCommandBuffer:command_buffer];
        [command_buffer commit];
    }
}


// ==== Draw ====
- (void)predrawOpenInBuffers {
    for (int i=0; i<_buffers.count; i++) {
        [_buffers[i] predrawOpen];
    }
}

- (void)predrawCloseInBuffers {
    for (int i=0; i<_buffers.count; i++) {
        [_buffers[i] predrawClose];
    }
}

- (void)drawCompletedInBuffers {
    for (int i=0; i<_buffers.count; i++) {
        [_buffers[i] drawCompleted];
    }
}


// ==== Depricated ====
- (DynamicBuffer*) newDynamicBufferWithVertexSizeOLD:(NSUInteger)vertex_size
                                      vertexCount:(NSUInteger)vertex_count
                                      storageMode:(MTLResourceOptions)storage_mode
{
    DynamicBuffer* buffer = [[DynamicBuffer alloc] initWithDevice:_device
                                                       vertexSize:vertex_size
                                                      vertexCount:vertex_count
                                                   andStorageMode:storage_mode];
    return buffer;
}


@end


// ================ Null Draw Routine ================
@implementation NullDrawRoutine
- (nonnull instancetype) initWithDeviceOLD:(nonnull id<MTLDevice>)device
                                library:(nonnull id<MTLLibrary>)library
{
    self = [super init];
    return self;
}
- (void) configureWithDrawablePixelFormatOLD:(MTLPixelFormat)pixel_format {}
- (void) createBufferWithVertexCountOLD:(NSUInteger)count {}

- (void) bindBufferOLD:(NSUInteger)buffer_index {}
- (nullable id<MTLBuffer>) writeBufferOpenOLD { return nil; }
- (void) writeBufferCloseOLD {}
- (void) drawInDrawable:(nonnull id<CAMetalDrawable>)drawable
        inCommandBuffer:(nonnull id<MTLCommandBuffer>)command_buffer {}
- (void) predrawOpenInBuffersOLD {}
- (void) predrawCloseInBuffersOLD {}
- (void) drawCompletedInBuffersOLD {}

- (nonnull instancetype)initWithDevice:(nonnull id<MTLDevice>)device library:(nonnull id<MTLLibrary>)library pixelFormat:(MTLPixelFormat)pixel_format { 
    self = [super init];
    return self;
}

- (void)bindBuffer:(NSUInteger)index {
}


- (void)createBufferWithVertexSize:(NSUInteger)vertex_size vertexCount:(NSUInteger)vertex_count storageMode:(MTLResourceOptions)storage_mode { 
}



@end
