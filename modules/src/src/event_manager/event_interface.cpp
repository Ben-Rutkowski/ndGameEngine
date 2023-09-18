#include "event_manager.hpp"

EventInterface::EventInterface()
    :event_manager{ nullptr } {}

void EventInterface::linkManager(EventManager* ptr) {
    event_manager = ptr;
}

void EventInterface::setCallback(Data data, EventCallType function) {
    function_array.addEntry(data, EventCall(function));
}

void EventInterface::operator()(Event* event) {
    EventCall function = function_array.get(event->getData());
    function(event);
}

void EventInterface::queueEvent(Module module_name, Data data) {
    event_manager->queueEvent(module_name, data);
}

void EventInterface::queueEvent2i(Module module_name, Data data, vec2i vector) {
    event_manager->queueEvent2i(module_name, data, vector);
}

void EventInterface::queueEvent2f(Module module_name, Data data, vec2 vector) {
    event_manager->queueEvent2f(module_name, data, vector);
}

void EventInterface::queueEvent4f(Module module_name, Data data, vec4 vector) {
    event_manager->queueEvent4f(module_name, data, vector);
}

void EventInterface::runEvent(Event* event) {
    event_manager->propogateEvent(event);
}

EventManager* EventInterface::ptr() { return event_manager; }