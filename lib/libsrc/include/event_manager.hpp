#ifndef EVENT_MANAGER_NEW_HPP
#define EVENT_MANAGER_NEW_HPP

#include "queue.hpp"
#include "event.hpp"

#define EVENT_QUEUE_LEN 16
#define EVENT2I_QUEUE_LEN 4
#define EVENT2F_QUEUE_LEN 4
#define EVENT4F_QUEUE_LEN 4

// ======== TYPES ========
typedef void(*EventCallTypeStatic)(void*, Event*);

template<typename T>
struct EventCallType {
    void(T::*callback)(Event*);

    void operator()(T* ptr, Event* event) 
        { ((*ptr).*callback)(event); }
};

// ========= Event Manager ========

class EventManager {
private:
    EventCallTypeStatic propogateEventCallback;
    void* app_ptr;

    Queue<Event, EVENT_QUEUE_LEN> event_queue;
    Queue<Event2i, EVENT2I_QUEUE_LEN> event2i_queue;
    Queue<Event2f, EVENT2F_QUEUE_LEN> event2f_queue;
    Queue<Event4f, EVENT4F_QUEUE_LEN> event4f_queue;

public:
    EventManager() {}
    void setCallback(void* ptr, EventCallTypeStatic callback) {
        propogateEventCallback = callback;
        app_ptr = ptr;
    }
    
public:
    void queueEvent(Module module_name, Data data)
        { event_queue.queue(Event(module_name, data)); }
    void queueEvent2i(Module module_name, Data data, vec2i vector)
        { event2i_queue.queue(Event2i(module_name, data, vector)); }
    void queueEvent2f(Module module_name, Data data, vec2 vector)
        { event2f_queue.queue(Event2f(module_name, data, vector)); }
    void queueEvent4f(Module module_name, Data data, vec4 vector)
        { event4f_queue.queue(Event4f(module_name, data, vector)); }

    void propogateEvent(Event* event) {
        propogateEventCallback(app_ptr, event);
    }

    void pollEvents() {
        Event* current_event;

        while (!event_queue.isEmpty()) {
            current_event = event_queue.getCurrent();
            propogateEvent(current_event);
            event_queue.clearCurrent();
        }

        while (!event2i_queue.isEmpty()) {
            current_event = event2i_queue.getCurrent();
            propogateEvent(current_event);
            event2i_queue.clearCurrent();
        }

        while (!event2f_queue.isEmpty()) {
            current_event = event2f_queue.getCurrent();
            propogateEvent(current_event);
            event2f_queue.clearCurrent();
        }
    }
};

#endif