#import "Buffers.h"

@implementation DynamicBufferNew
{
    id<MTLDevice> _device;
    id<MTLBuffer> _buffer;

    NSUInteger _vertex_size;
    NSUInteger _vertex_count;

    NSUInteger _reference_count;

    // --- Thread Controllers ---
    dispatch_semaphore_t _is_being_read_semaphore;
}


// ================ Initialization ================
- (instancetype) initWithDevice:(id<MTLDevice>)device 
                     vertexSize:(NSUInteger)vertex_size 
                    vertexCount:(NSUInteger)vertex_count 
{
    self = [super init];
    if (self) {
        _device       = device;
        _vertex_size  = vertex_size;
        _vertex_count = vertex_count;
        _reference_count = 0;

        _buffer = [_device newBufferWithLength:_vertex_size*_vertex_count 
                                       options:MTLResourceStorageModeManaged];

        _is_being_read_semaphore    = dispatch_semaphore_create(1);
    }
    return self;
}

- (NSUInteger) getVertexCount {
    return _vertex_count;
}


// ================ Draw ================
- (void) predrawOpen {
    // dispatch_semaphore_wait(_is_being_read_semaphore, DISPATCH_TIME_FOREVER);
    // dispatch_semaphore_signal(_is_being_read_semaphore);
    _reference_count += 1;
    // printf("-> %lu\n", _reference_count);
}

- (id<MTLBuffer>) drawTap {
    return _buffer;
}

- (id<MTLBuffer>) drawRelay {
    return _buffer;
}

- (void) predrawClose {
    // dispatch_semaphore_signal(_is_being_read_semaphore);
}

- (void) drawCompleted {
    _reference_count -= 1;
    // printf("<- %lu\n", _reference_count);
}


// ================ Write ================
- (id<MTLBuffer>) writeOpen {
    dispatch_semaphore_wait(_is_being_read_semaphore, DISPATCH_TIME_FOREVER);
    // printf("WRITE: %lu\n", _reference_count);
    return _buffer;
}

- (void) writeClose {

}

- (void) writeCloseWithRange:(NSRange)range {
    dispatch_semaphore_signal(_is_being_read_semaphore);
    [_buffer didModifyRange:range];
}


// ================ Debug ================
- (void) debug:(NSUInteger)num_vertices {
    printf("New Dynamic Buffer Debug::NOT IMPLEMENTED");
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
- (void) writeCloseWithRange:(NSRange)range {}
- (id<MTLBuffer>) writeOpen { return nil; }
- (void) debug:(NSUInteger)num_vertices {}
@end
