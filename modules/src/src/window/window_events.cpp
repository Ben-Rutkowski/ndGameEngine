#include "window.hpp"

void ndWindowModule::runEvent(ndEvent* event) {
    switch (event->getOp()) {
    case Operation::BEGIN_LOOP:        onBeginLoop(event); break;
    case Operation::START_FRAME:       onStartFrame(event); break;
    case Operation::DRAW_WINDOW_FRAME: onDrawWindowFrame(event); break;
    case Operation::DRAW:              onDraw(event); break;
    case Operation::END_FRAME:         onEndFrame(event); break;
    case Operation::CLOSE_APP:         onCloseApp(event); break;
    default: break;
    }
}

void ndWindowModule::onBeginLoop(ndEvent* event) {
    nd_window.setClearColor(0.0, 0.5, 0.5, 1.0);
    pollEventsCocoa();
    nd_window.showWindow();
    event->print(module_name);
}

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
