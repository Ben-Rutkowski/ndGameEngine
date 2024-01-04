/*
    Thread Racing:
                       |Cannot|             |     Cannot     |
                       | Edit |             |      Swap      |
        GPU|           ( Draw )        GPU|           ( Draw )
  Draw  ---|-------------------        ---|-------------------
        CPU| ( Predraw )               CPU| ( Predraw )
 
                              |Cannot |
                              | Draw, |                      |   Cannot    |
                              |Predraw|                      |    Edit     |
        GPU|           ( Copy )               GPU|           ( Copy )
 Resize ---|--------------------------        ---|--------------------------
        CPU| ( Presize )      ( Swap  )       CPU| ( Presize )      ( Swap )
 
                                  |Cannot|
             |Cannot|             | Copy,|
             | Draw |             | Swap |
        GPU|                 GPU|
  Edit  ---|---------        ---|---------
        CPU| ( Edit )        CPU| ( Edit )
*/

#import "Buffers.h"

@implementation DynamicBuffer
{
    id<MTLBuffer> _buffer;
    NSUInteger    _vertex_count;
    
    id<MTLBuffer> _new_buffer;
    NSUInteger    _new_vertex_count;
    
    dispatch_semaphore_t _predraw_semaphore;
    dispatch_semaphore_t _draw_semaphore;
    dispatch_semaphore_t _copy_semaphore;
    dispatch_semaphore_t _swap_semaphore;
    dispatch_semaphore_t _edit_semaphore;
    
//    --- Depricated ---
//    dispatch_semaphore_t _OLDresize_semaphore;
//    dispatch_semaphore_t _OLDin_use_semaphore;
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
        
        _predraw_semaphore = dispatch_semaphore_create(1);
        _draw_semaphore    = dispatch_semaphore_create(1);
        _copy_semaphore    = dispatch_semaphore_create(1);
        _swap_semaphore    = dispatch_semaphore_create(1);
        _edit_semaphore    = dispatch_semaphore_create(1);
        
//        --- Depricated ---
//        _OLDresize_semaphore = dispatch_semaphore_create(1);
//        _OLDin_use_semaphore = dispatch_semaphore_create(1);
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
        [command_buffer addScheduledHandler:^(id<MTLCommandBuffer> nonnull) {
            
        }];
            
        [command_buffer addCompletedHandler:^(id<MTLCommandBuffer> nonnull) {
            [block_buffer_self rotateNewBuffer];
        }];
        
        [command_buffer commit];
    }
}

- (void) beforeCopy {
    dispatch_semaphore_wait(_edit_semaphore, DISPATCH_TIME_FOREVER);
    NSLog(@"  Wait   : EDIT");
}

- (void) rotateNewBuffer {
    dispatch_semaphore_wait(_predraw_semaphore, DISPATCH_TIME_FOREVER);
    NSLog(@"  Wait   : PREDRAW");
    dispatch_semaphore_wait(_predraw_semaphore, DISPATCH_TIME_FOREVER);
    NSLog(@"  Wait   : DRAW");
    
//    dispatch_semaphore_wait(_OLDresize_semaphore, DISPATCH_TIME_FOREVER);
//    NSLog(@"   Wait : resize semaphore");
    
    [_buffer setPurgeableState:MTLPurgeableStateEmpty];
    [_buffer release];
    _buffer       = _new_buffer;
    _vertex_count = _new_vertex_count;
    
    dispatch_semaphore_signal(_predraw_semaphore);
    NSLog(@"  Finish : PREDRAW");
    dispatch_semaphore_signal(_draw_semaphore);
    NSLog(@"  Finish : DRAW");
    dispatch_semaphore_signal(_edit_semaphore);
    NSLog(@"  Finish : EDIT");
    
//    dispatch_semaphore_signal(_OLDresize_semaphore);
//    NSLog(@"   Finish : resize semaphore");
}


// ==== Access ====
- (id<MTLBuffer>) drawTap {
    return _buffer;
}

- (void) beforeDraw {
    dispatch_semaphore_wait(_swap_semaphore, DISPATCH_TIME_FOREVER);
    NSLog(@"  Wait   : SWAP");
    
//    dispatch_semaphore_wait(_OLDresize_semaphore, DISPATCH_TIME_FOREVER);
//    NSLog(@"   Wait : resize semaphore");
}

- (void) drawUntapScheduled {
    dispatch_semaphore_wait(_edit_semaphore, DISPATCH_TIME_FOREVER);
    NSLog(@"  Wait   : EDIT");
    
//    dispatch_semaphore_wait(_OLDin_use_semaphore, DISPATCH_TIME_FOREVER);
//    NSLog(@"   Wait : use semaphore");
}

- (void) drawUntapCompleted  {
    dispatch_semaphore_signal(_swap_semaphore);
    NSLog(@"  Finish : SWAP");
    dispatch_semaphore_signal(_edit_semaphore);
    NSLog(@"  Finish : EDIT");
    
//    dispatch_semaphore_signal(_OLDresize_semaphore);
//    NSLog(@"   Finish : resize semaphore");
//    dispatch_semaphore_signal(_OLDin_use_semaphore);
//    NSLog(@"   Finish : use semaphore");
}

- (id<MTLBuffer>) editTap {
    dispatch_semaphore_wait(_draw_semaphore, DISPATCH_TIME_FOREVER);
    NSLog(@"  Wait   : DRAW");
    dispatch_semaphore_wait(_copy_semaphore, DISPATCH_TIME_FOREVER);
    NSLog(@"  Wait   : COPY");
    dispatch_semaphore_wait(_swap_semaphore, DISPATCH_TIME_FOREVER);
    NSLog(@"  Wait   : SWAP");
    
//    dispatch_semaphore_wait(_OLDin_use_semaphore, DISPATCH_TIME_FOREVER);
//    NSLog(@"   Wait : use semaphore");
    return _buffer;
}

- (void) editUntap {
    dispatch_semaphore_signal(_draw_semaphore);
    NSLog(@"  Finish : DRAW");
    dispatch_semaphore_signal(_copy_semaphore);
    NSLog(@"  Finish : COPY");
    dispatch_semaphore_signal(_swap_semaphore);
    NSLog(@"  Finish : SWAP");
    
//    dispatch_semaphore_signal(_OLDin_use_semaphore);
//    NSLog(@"   Finish : use semaphore");
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
