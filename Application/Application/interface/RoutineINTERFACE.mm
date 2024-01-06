#import "cocoa_interface.hpp"
#import "DrawRoutines.h"

ndRoutine::ndRoutine(void* routine_ptr, unsigned long index)
:routine_ptr_COCOA(routine_ptr), routine_index(index) {}

void ndRoutine::bindBuffer(unsigned int buffer_index) {
    id<DrawRoutineProtocol> draw_routine = (id<DrawRoutineProtocol>)routine_ptr_COCOA;
    [draw_routine bindBufferOLD:buffer_index];
}

void ndRoutine::createBuffer(unsigned int vertex_count) {
    id<DrawRoutineProtocol> draw_routine = (id<DrawRoutineProtocol>)routine_ptr_COCOA;
    [draw_routine createBufferWithVertexCountOLD:vertex_count];
}

void* ndRoutine::writeBufferOpen() {
    id<DrawRoutineProtocol> draw_routine = (id<DrawRoutineProtocol>)routine_ptr_COCOA;
    id<MTLBuffer> buffer = [draw_routine writeBufferOpenOLD];
    return buffer.contents;
}

void ndRoutine::writeBufferClose() {
    id<DrawRoutineProtocol> draw_routine = (id<DrawRoutineProtocol>)routine_ptr_COCOA;
    [draw_routine writeBufferCloseOLD];
}

unsigned long ndRoutine::index() {
    return routine_index;
}
