#include "event_manager.hpp"

EventInterfaceOld::EventInterfaceOld()
    :event_manager{ nullptr } {}

void EventInterfaceOld::linkManager(EventManagerOld* ptr) {
    event_manager = ptr;
}

void EventInterfaceOld::setCallback(Data data, EventCallTypeOld function) {
    function_array.addEntry(data, EventCallOld(function));
}

void EventInterfaceOld::operator()(Event* event) {
    EventCallOld function = function_array.get(event->getData());
    function(event);
}

void EventInterfaceOld::queueEvent(Module module_name, Data data) {
    event_manager->queueEvent(module_name, data);
}

void EventInterfaceOld::queueEvent2i(Module module_name, Data data, vec2i vector) {
    event_manager->queueEvent2i(module_name, data, vector);
}

void EventInterfaceOld::queueEvent2f(Module module_name, Data data, vec2 vector) {
    event_manager->queueEvent2f(module_name, data, vector);
}

void EventInterfaceOld::queueEvent4f(Module module_name, Data data, vec4 vector) {
    event_manager->queueEvent4f(module_name, data, vector);
}

void EventInterfaceOld::runEvent(Event* event) {
    event_manager->propogateEvent(event);
}

EventManagerOld* EventInterfaceOld::ptr() { return event_manager; }