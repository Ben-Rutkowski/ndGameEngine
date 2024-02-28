#import "cocoa_interface.hpp"
#import "DrawRoutines.h"

ndRoutine::ndRoutine(void* routine_ptr, unsigned long index)
:routine_ptr_COCOA(routine_ptr), routine_index(index) {}

void ndRoutine::bindBuffer(unsigned int buffer_index) {
    DrawRoutineTemplate* draw_routine = (DrawRoutineTemplate*)routine_ptr_COCOA;
    [draw_routine bindBuffer:buffer_index];
}

void ndRoutine::createPublicBuffer(unsigned int vertex_size, unsigned int vertex_count) {
    DrawRoutineTemplate* draw_routine = (DrawRoutineTemplate*)routine_ptr_COCOA;
    [draw_routine createBufferWithVertexSize:vertex_size
                                 vertexCount:vertex_count];
}

void ndRoutine::createAuxBuffer(unsigned int vertex_size, unsigned int vertex_count) {
    DrawRoutineTemplate* draw_routine = (DrawRoutineTemplate*)routine_ptr_COCOA;
    [draw_routine createBufferWithVertexSize:vertex_size
                                 vertexCount:vertex_count];
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

unsigned long ndRoutine::index() {
    return routine_index;
}

void ndRoutine::debug(unsigned int vertex_count) {
    DrawRoutineTemplate* draw_routine = (DrawRoutineTemplate*)routine_ptr_COCOA;
    [draw_routine debugBuffer:vertex_count];
}
