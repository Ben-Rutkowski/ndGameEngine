#include "edit_space.hpp"

void EditSpace::setCallbacks() {
    event_interface.setCallback(Data::BEGIN_LOOP, PACK(EditSpace::onBeginLoop));
}

void EditSpace::runEventEditSpace(Event* event) {
    event_interface(event);
}

// === On Events ===
void EditSpace::onBeginLoop(Event* event) {
    event->print(module_name);
}