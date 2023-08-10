#include "event_manager.hpp"

// === Constructors and Initialization ===
EventManager::EventManager() {}

void EventManager::setCallback(EventCall event_call) {
    propogateEventCall = event_call;
}

// === Interface ===
void EventManager::queueEvent(EventCode code) {
    event_queue.queue(Event(code));
}

void EventManager::queueEvent2i(EventCode code, int arg0, int arg1) {
    event2i_queue.queue(Event2i(code, vec2i(arg0, arg1)));
}

void EventManager::queueEvent2i(EventCode code, vec2i vector) {
    event2i_queue.queue(Event2i(code, vector));
}

void EventManager::runEvent(Event* event) {
    propogateEventCall(event);
}

void EventManager::pollEvents() {
    Event* current_event;

    while (!event_queue.isEmpty()) {
        current_event = event_queue.getCurrent();
        propogateEventCall(current_event);
        event_queue.clearCurrent();
    }

    while (!event2i_queue.isEmpty()) {
        current_event = event2i_queue.getCurrent();
        propogateEventCall(current_event);
        event2i_queue.clearCurrent();
    }
}


// ================ EventInterface ================


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