#include "event_manager.hpp"

// === Constructors and Initialization ===
EventManager::EventManager() {}

void EventManager::setCallback(EventCall event_call) {
    propogateEventCall = event_call;
}

// === Interface ===
void EventManager::queueEvent(Module module_name, Data data) {
    event_queue.queue(Event(module_name, data));
}

void EventManager::queueEvent2i(Module module_name, Data data, vec2i vector) {
    event2i_queue.queue(Event2i(module_name, data, vector));
}

void EventManager::queueEvent2f(Module module_name, Data data, vec2 vector) {
    event2f_queue.queue(Event2f(module_name, data, vector));
}

void EventManager::queueEvent4f(Module module_name, Data data, vec4 vector) {
    event4f_queue.queue(Event4f(module_name, data, vector));
}

void EventManager::propogateEvent(Event* event) {
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

    while (!event2f_queue.isEmpty()) {
        current_event = event2f_queue.getCurrent();
        propogateEventCall(current_event);
        event2f_queue.clearCurrent();
    }
}