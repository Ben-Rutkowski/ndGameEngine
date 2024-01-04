#import "cocoa_interface.hpp"
#import "Buffers.h"

ndBuffer::ndBuffer(void* buffer_ptr)
:buffer_ptr_COCOA(buffer_ptr) {}

void* ndBuffer::editTap() {
    DynamicBuffer* dynamic_buffer = (DynamicBuffer*)buffer_ptr_COCOA;
    id<MTLBuffer> metal_buffer = [dynamic_buffer editTap];
    return metal_buffer.contents;
}

void ndBuffer::editUntap() {
    DynamicBuffer* dynamic_buffer = (DynamicBuffer*)buffer_ptr_COCOA;
    [dynamic_buffer editUntap];
}

unsigned long ndBuffer::size() {
    DynamicBuffer* dynamic_buffer = (DynamicBuffer*)buffer_ptr_COCOA;
    id<MTLBuffer> metal_buffer = [dynamic_buffer editTap];
    return metal_buffer.length;
}

void ndBuffer::debug() {
    DynamicBuffer* dynamic_buffer = (DynamicBuffer*)buffer_ptr_COCOA;
    [dynamic_buffer debug];
}
