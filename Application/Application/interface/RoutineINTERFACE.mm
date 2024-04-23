#include <Foundation/Foundation.h>
#include <iostream>
#include "DrawRoutineTemplates.h"
#import "cocoa_interface.hpp"
#import "DrawRoutines.h"

ndRoutine::ndRoutine(void* routine_ptr, unsigned long index)
:routine_ptr_COCOA(routine_ptr), routine_index(index) {}

ndRoutine::ndRoutine()
:routine_ptr_COCOA(0), routine_index(0) {}

void ndRoutine::bindBuffer(unsigned int buffer_index) {
    DrawRoutineTemplate* draw_routine = (DrawRoutineTemplate*)routine_ptr_COCOA;
    [draw_routine bindBuffer:buffer_index];
}

void ndRoutine::createBuffer(Buffer_T buffer_type, unsigned int vertex_size, unsigned int vertex_count) {
    DrawRoutineTemplate* draw_routine = (DrawRoutineTemplate*)routine_ptr_COCOA;
    [draw_routine createBufferWithVertexSize:vertex_size
                                 vertexCount:vertex_count
                                  bufferType:buffer_type];
}

void* ndRoutine::writeBufferOpen() {
    DrawRoutineTemplate* draw_routine = (DrawRoutineTemplate*)routine_ptr_COCOA;
    id<MTLBuffer> buffer = [draw_routine writeBufferOpen];
    return buffer.contents;
}

void ndRoutine::writeBufferClose() {
    DrawRoutineTemplate* draw_routine = (DrawRoutineTemplate*)routine_ptr_COCOA;
    [draw_routine writeBufferClose];
}

void ndRoutine::writeBufferModify(int loc, int len) {
    NSRange range = NSMakeRange(loc, len);
    DrawRoutineTemplate* draw_routine = (DrawRoutineTemplate*)routine_ptr_COCOA;
    [draw_routine writeBufferCloseWithRange:range];
}

unsigned long ndRoutine::index() {
    return routine_index;
}

void ndRoutine::debug(unsigned int vertex_count) {
    DrawRoutineTemplate* draw_routine = (DrawRoutineTemplate*)routine_ptr_COCOA;
    [draw_routine debugBuffer:vertex_count];
}

void ndRoutine::debug() {
    std::cout << routine_ptr_COCOA << std::endl;
}
