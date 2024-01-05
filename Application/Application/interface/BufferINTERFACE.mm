#import "cocoa_interface.hpp"
#import "Buffers.h"

ndBuffer::ndBuffer(void* buffer_ptr)
:buffer_ptr_COCOA(buffer_ptr) {}

void* ndBuffer::editTap() {
    ResizableBuffer* dynamic_buffer = (ResizableBuffer*)buffer_ptr_COCOA;
    id<MTLBuffer> metal_buffer = [dynamic_buffer editTap];
    return metal_buffer.contents;
}

void ndBuffer::editUntap() {
    ResizableBuffer* dynamic_buffer = (ResizableBuffer*)buffer_ptr_COCOA;
    [dynamic_buffer editUntap];
}

unsigned long ndBuffer::size() {
    ResizableBuffer* dynamic_buffer = (ResizableBuffer*)buffer_ptr_COCOA;
    id<MTLBuffer> metal_buffer = [dynamic_buffer editTap];
    return metal_buffer.length;
}

void ndBuffer::debug() {
    ResizableBuffer* dynamic_buffer = (ResizableBuffer*)buffer_ptr_COCOA;
    [dynamic_buffer debug];
}
