#ifndef EVENT_MANAGER_HPP
#define EVENT_MANAGER_HPP

#include "queue.hpp"
#include "event.hpp"

#define DEFAULT_QUEUE_LENGTH = 64;

typedef void(*EventCallbackType)(void*, ndEvent*);

class ndEventManager {
// --- Attributes ---
private:
    EventCallbackType propgateEventCallback;
    void* app_module_ptr;

    Queue<ndEvent> event_queue;

// --- Constructor ---
public:
    
};

// // ======== TYPES ========
// typedef void(*EventCallTypeStatic)(void*, Event*);

// template<typename T>
// struct EventCallType {
//     void(T::*callback)(Event*);

//     void operator()(T* ptr, Event* event) 
//         { ((*ptr).*callback)(event); }
// };

// // ========= Event Manager ========

// public:
//     EventManager() {}
//     void setCallback(void* ptr, EventCallTypeStatic callback) {
//         propogateEventCallback = callback;
//         app_ptr = ptr;
//     }
    
// public:
// // ======== Queueing ========
//     void queueEvent(Module module_name, Data data)
//         { event_queue.queue(Event(module_name, data)); }
//     void queueEvent4f(Module module_name, Data data, vec4 vector)
//         { event4f_queue.queue(Event4f(module_name, data, vector)); }
//     void queueEvent4i(Module module_name, Data data, vec4i vector)
//         { event4i_queue.queue(Event4i(module_name, data, vector)); }

// // ======== Propogating ========
//     void propogateEvent(Module module_name, Data data) {
//         current_event = Event(module_name, data);
//         propogateEventCallback(app_ptr, &current_event);
//     }
//     void propogateEvent4f(Module module_name, Data data, vec4 vector) {
//         current_event4f = Event4f(module_name, data, vector);
//         propogateEventCallback(app_ptr, &current_event4f);
//     }
//     void propogateEvent4i(Module module_name, Data data, vec4i vector) {
//         current_event4i = Event4i(module_name, data, vector);
//         propogateEventCallback(app_ptr, &current_event4i);
//     }


//     // void queueEvent2f(Module module_name, Data data, vec2 v)
//     //     { queueEvent4f(module_name, data, vec4({v[0], v[1], 0.0f, 0.0f})); }
//     // void queueEvent2i(Module module_name, Data data, vec2i vector)
//     //     { event2i_queue.queue(Event2i(module_name, data, vector)); }


//     // void queueEvent2f(Module module_name, Data data, vec2 vector)
//     //     { event2f_queue.queue(Event2f(module_name, data, vector)); }

//     // void propogateEvent(Event* event) {
//     //     propogateEventCallback(app_ptr, event);
//     // }

//     void pollEvents() {
//         Event* current_event;

//         while (!event_queue.isEmpty()) {
//             current_event = event_queue.getCurrent();
//             // propogateEvent(current_event);
//             propogateEventCallback(app_ptr, current_event);
//             event_queue.clearCurrent();
//         }

//         while (!event4f_queue.isEmpty()) {
//             current_event = event4f_queue.getCurrent();
//             // propogateEvent(current_event);
//             propogateEventCallback(app_ptr, current_event);
//             event4f_queue.clearCurrent();
//         }

//         while (!event4i_queue.isEmpty()) {
//             current_event = event4i_queue.getCurrent();
//             // propogateEvent(current_event);
//             propogateEventCallback(app_ptr, current_event);
//             event4i_queue.clearCurrent();
//         }

//         // while (!event2i_queue.isEmpty()) {
//         //     current_event = event2i_queue.getCurrent();
//         //     propogateEvent(current_event);
//         //     event2i_queue.clearCurrent();
//         // }

//         // while (!event2f_queue.isEmpty()) {
//         //     current_event = event2f_queue.getCurrent();
//         //     propogateEvent(current_event);
//         //     event2f_queue.clearCurrent();
//         // }
//     }
// };

#endif