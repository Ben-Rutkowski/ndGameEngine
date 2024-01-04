#import "Buffers.h"

@implementation DynamicBuffer
{
    id<MTLBuffer> _buffer;
    NSUInteger    _vertex_count;
    
    id<MTLBuffer> _new_buffer;
    NSUInteger    _new_vertex_count;
    
    dispatch_semaphore_t _resize_semaphore;
    dispatch_semaphore_t _in_use_semaphore;
}


// ==== Configure ====
- (nonnull instancetype) initWithDevice:(nonnull id<MTLDevice>)device
                               dataSize:(NSUInteger)data_size
                            vertexCount:(NSUInteger)vertex_count
                         andStorageMode:(MTLResourceOptions)storage_mode
{
    self = [super init];
    if (self) {
        _buffer = [device newBufferWithLength:data_size
                                      options:storage_mode];
        
        _vertex_count     = vertex_count;
        _resize_semaphore = dispatch_semaphore_create(1);
        _in_use_semaphore = dispatch_semaphore_create(1);
    }
    
    return self;
}


// ==== Size ====
- (NSUInteger) getVertexCount {
    return _vertex_count;
}

- (void) expandToDataSize:(NSUInteger)data_size
           andVertexCount:(NSUInteger)vertex_count
               withDevice:(nonnull id<MTLDevice>)device
           inCommandQueue:(nonnull id<MTLCommandQueue>)command_queue
{
    _new_buffer = [device newBufferWithLength:data_size
                                      options:_buffer.resourceOptions];
    _new_vertex_count = vertex_count;
    
    @autoreleasepool {
        id<MTLCommandBuffer>      command_buffer = [command_queue commandBuffer];
        id<MTLBlitCommandEncoder> blit_command   = [command_buffer blitCommandEncoder];
        
        [blit_command copyFromBuffer:_buffer
                        sourceOffset:0
                            toBuffer:_new_buffer
                   destinationOffset:0
                                size:_buffer.length];
        
        [blit_command endEncoding];
        
        __block DynamicBuffer* block_buffer_self = self;
        [command_buffer addCompletedHandler:^(id<MTLCommandBuffer> buffer) {
            [block_buffer_self rotateNewBuffer];
        }];
        
        [command_buffer commit];
    }
}

- (void) rotateNewBuffer {
    dispatch_semaphore_wait(_resize_semaphore, DISPATCH_TIME_FOREVER);
//    NSLog(@"Wait : resize semaphore");
    [_buffer setPurgeableState:MTLPurgeableStateEmpty];
    [_buffer release];
    _buffer       = _new_buffer;
    _vertex_count = _new_vertex_count;
    dispatch_semaphore_signal(_resize_semaphore);
//    NSLog(@"Finish : resize semaphore");
}


// ==== Access ====
- (id<MTLBuffer>) drawTap {
    return _buffer;
}

- (void) beforeDraw {
//    NSLog(@"(before wait)");
    dispatch_semaphore_wait(_resize_semaphore, DISPATCH_TIME_FOREVER);
//    NSLog(@"   Wait : resize semaphore");
}

- (void) drawUntapScheduled {
//    NSLog(@"(before wait)");
    dispatch_semaphore_wait(_in_use_semaphore, DISPATCH_TIME_FOREVER);
//    NSLog(@"   Wait : use semaphore");
}

- (void) drawUntapCompleted  {
//    NSLog(@"(before finish)");
    dispatch_semaphore_signal(_resize_semaphore);
//    NSLog(@"   Finish : resize semaphore");
    dispatch_semaphore_signal(_in_use_semaphore);
//    NSLog(@"   Finish : use semaphore");
}

- (id<MTLBuffer>) editTap {
//    NSLog(@"(before wait)");
    dispatch_semaphore_wait(_in_use_semaphore, DISPATCH_TIME_FOREVER);
//    NSLog(@"   Wait : use semaphore");
    return _buffer;
}

- (void) editUntap {
//    NSLog(@"(before finish)");
    dispatch_semaphore_signal(_in_use_semaphore);
//    NSLog(@"Finish : use semaphore");
}


// ==== Debug ====
- (void) debug {
    NSLog(@"Buffer size : %lu", _buffer.length);
    NSLog(@"Buffer vertex count : %lu", _vertex_count);
    
    float* vert = _buffer.contents;
    
    for (int i=0; i<4; i++) {
        NSLog(@"%f", vert[i]);
    }
}

@end
