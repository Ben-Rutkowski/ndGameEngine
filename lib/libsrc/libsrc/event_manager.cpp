#include "event_manager.hpp"
#include "cocoa_interface.hpp"
#include "common.hpp"

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


// ================ Callbacks ================
void EventManager::cocoaCallbackAnchor(void* event_manager_ptr, unsigned int type) {
    // std::cout << "Callback Achor" << std::endl;
    EventManager* this_ptr = (EventManager*)event_manager_ptr;

    switch (type) {
        case CocoaCB_Resize: {
            // this_ptr->queueEvent(
            //     ndEvent(Module::COCOA, Operation::DEBUG)
            // );
            this_ptr->propogateEventImmediate(
                ndEvent(Module::COCOA, Operation::RESIZE_FRAME)
            );
        break;
        } 

        default: break;
    }
}

void EventManager::cocoaCallbackVI2Anchor(void* event_manager_ptr, unsigned int type, int a, int b) {
    std::cout << "CallbackVI2 Achor" << std::endl;
    // EventManager* this_ptr = (EventManager*)event_manager_ptr;
}
