#include "edit_space.hpp"

Module EditSpace::module_name = Module::EDIT_SPACE;

EditSpace::EditSpace() {
    setCallbacks();
}

void EditSpace::setManagerPtr(EventManager* ptr) {
    event_interface.linkManager(ptr);
}