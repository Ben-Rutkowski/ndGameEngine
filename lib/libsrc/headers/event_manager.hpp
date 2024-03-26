#ifndef EVENT_MANAGER_HPP
#define EVENT_MANAGER_HPP

#include "queue.hpp"
#include "event.hpp"

#define DEFAULT_QUEUE_LENGTH 64

typedef void(*EventCallbackTypeStatic)(void*, ndEvent*);

class EventManager {
// --- Attributes ---
private:
    void* app_module_ptr;   
    EventCallbackTypeStatic propogateEventCallback;

    Queue<ndEvent> event_queue;

// --- Initialization ---
public:
    EventManager();
    void linkCallback(void* app_ptr_in, EventCallbackTypeStatic callback);

// --- Event Interface ---
public:
    void queueEvent(ndEvent &&event);
    void pollEvents();
    void pollEvents(int amount);

    void propogateEventImmediate(ndEvent &&event);

// --- Private ---
private:
    void propogateCurrentQueuedEvent();

// --- Debugging ---
public:
    void debug() { event_queue.debug(); }

// --- Cocoa Callbacks ---
public: 
    static void cocoaCallbackAnchor(void* event_manager_ptr, unsigned int type);
    static void cocoaCallbackVI2Anchor(void* event_manager_ptr, unsigned int type, int a, int b);
};

#endif
