#include "event_manager.hpp"

EventInterface::EventInterface()
    :event_manager{ nullptr } {}

void EventInterface::linkManager(EventManager* ptr) {
    event_manager = ptr;
}

void EventInterface::setCallback(Data data, EventCallType function) {
    function_array[(int)data] = EventCall(function);
}

void EventInterface::operator()(Event* event) {
    int index = event->getIndex();
    EventCall function = function_array[index];
    function(event);
}

void EventInterface::queueEvent(EventCode code) {
    event_manager->queueEvent(code);
}

void EventInterface::queueEvent2i(EventCode code, int arg0, int arg1) {
    event_manager->queueEvent2i(code, arg0, arg1);
}

void EventInterface::queueEvent2i(EventCode code, vec2i vector) {
    event_manager->queueEvent2i(code, vector);
}

void EventInterface::runEvent(Event* event) {
    event_manager->runEvent(event);
}