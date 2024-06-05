#include "cocoa_interface.hpp"
#include "window.hpp"

void ndWindowModule::runEvent(ndEvent* event) {
    switch (event->getOp()) {
    case Operation::DEBUG:             onDebug(event); break;
    case Operation::BEGIN_START_UP:    onBeginStartUp(event); break;
    case Operation::END_START_UP:      onEndStartUp(event); break;
    case Operation::START_FRAME:       onStartFrame(event); break;
    case Operation::DRAW_WINDOW_FRAME: onDrawWindowFrame(event); break;
    case Operation::DRAW:              onDraw(event); break;
    case Operation::RESIZE_FRAME:      onResizeFrame(event); break;
    case Operation::END_FRAME:         onEndFrame(event); break;
    case Operation::CLOSE_APP:         onCloseApp(event); break;
    default: break;
    }

    if (edit_space) {
        edit_space->runEvent(event);
    } else {
        std::cout << "No edit space " << std::endl;
    }
    
}

// ================ StartUp ================
void ndWindowModule::onBeginStartUp(ndEvent* event) {
    event->print(module_name);
}

void ndWindowModule::onEndStartUp(ndEvent* event) {
    nd_window.showWindow();
    // nd_window.armRoutine(edit_space_draw_rouitne);
    pollEventsCocoa();
}


// ================ Frame ================
void ndWindowModule::onStartFrame(ndEvent* event) {
}

void ndWindowModule::onDrawWindowFrame(ndEvent* event) {
}

void ndWindowModule::onDraw(ndEvent* event) {
    nd_window.drawView();
}

void ndWindowModule::onEndFrame(ndEvent* event) {
}

void ndWindowModule::onCloseApp(ndEvent* event) {
    event->print(module_name);
}


// ================ State Changes ================
void ndWindowModule::onResizeFrame(ndEvent* event) {
}
