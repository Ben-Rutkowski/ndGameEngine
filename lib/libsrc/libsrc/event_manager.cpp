#include "event_manager.hpp"

// --- Initialization ---
EventManager::EventManager() 
    :event_queue(DEFAULT_QUEUE_LENGTH) {}

void EventManager::linkCallback(void* app_ptr_in, EventCallbackTypeStatic callback) {
    app_module_ptr = app_ptr_in;
    propogateEventCallback = callback;
}   

// --- Event Interface ---
void EventManager::queueEvent(ndEvent &&event) {
    event_queue.queue(event);
}

void EventManager::pollEvents() {
    while (event_queue.hasElements()) {
        propogateCurrentQueuedEvent();
    }
}

void EventManager::pollEvents(int amount) {
    int i = 0;
    while (event_queue.hasElements() && i<amount) {
        propogateCurrentQueuedEvent();
        i++;
    }
}

void EventManager::propogateEventImmediate(ndEvent &&event) {
    event_queue.queueFront(event);
    propogateCurrentQueuedEvent();
}

// --- Private ---
void EventManager::propogateCurrentQueuedEvent() {
    ndEvent* current_event = event_queue.getCurrent();
    propogateEventCallback(app_module_ptr, current_event);
    event_queue.dequeueCurrent();
}