#include "window.hpp"
#include "draw_routine_indices.h"

// === Initialization ===
ndWindowModule::ndWindowModule(int width, int height, const char* title)
    :ndModuleImplement<ndWindowStateLen>(Module::WINDOW),
    nd_window(width, height, title),
    edit_space{ nullptr }
{

}

void ndWindowModule::linkEditSpace(ndModule* edit_space_ptr) {
    edit_space = edit_space_ptr;
    // edit_space_draw_rouitne = nd_window.createDrawRoutine(DrawRoutineEditSpace);
    edit_space_draw_rouitne = nd_window.createDrawRoutine(DrawRoutineKindDebug);
    edit_space->setDrawRoutine(edit_space_draw_rouitne);
}

bool ndWindowModule::requestBool(Request request) {
    switch (request) {
    case Request::SHOULD_CLOSE: return nd_window.shouldClose();
    default: return false;
    }
}

float ndWindowModule::requestFloat(Request request) {
    switch (request) {
    case Request::FRAME_DELTA: return 0.0f;
    default: return 0.0f;
    }
}
