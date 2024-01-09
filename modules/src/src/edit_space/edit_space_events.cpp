#include "edit_space.hpp"

void ndEditSapceModule::runEvent(ndEvent* event) {
    switch (event->getOp()) {
    case Operation::DEBUG:          onDebug(event); break;
    case Operation::BEGIN_START_UP: onBeginStartUp(event); break;
    case Operation::END_START_UP:   onEndStartUp(event); break;
    case Operation::START_FRAME:    onStartFrame(event); break;
    case Operation::END_FRAME:      onEndFrame(event); break;
    default: break;
    }
}


// ================ Debug ================
void ndEditSapceModule::onDebug(ndEvent* event) {

}


// ================ App Loop ================
void ndEditSapceModule::onBeginStartUp(ndEvent* event) {
    event->print(module_name);
}

void ndEditSapceModule::onEndStartUp(ndEvent* event) {
    event->print(module_name);
}

void ndEditSapceModule::onStartFrame(ndEvent* event) {
    event->print(module_name);
}

void ndEditSapceModule::onEndFrame(ndEvent* event) {
    event->print(module_name);
}