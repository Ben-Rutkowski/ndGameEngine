#include "window.hpp"

// === Initialization ===
ndWindowModule::ndWindowModule(int width, int height, const char* title)
    :ndModule<ndWindowStateLen>(Module::WINDOW),
    nd_window(width, height, title) {}

void ndWindowModule::linkEditSpace(ndModule* edit_space_ptr) {
    throw std::runtime_error("Not Implemented ndWindowModule::linkEditSpace");
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