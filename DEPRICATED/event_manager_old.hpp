/*

The Event Manager distributes Events throughout the application and
is contained inside the Application layer as. Every other module 
has a pointer to the manager. 

Events can be queued or ran individualy:
- When queued, Events exist in a stack of predetermined size. 
- When ran individually, Events exist in run function.

ATTRIBUTES
-- runEvent: The callback function for the application propogateEvent
    anchor function. 
-- event_queue: A queue of normal event objects. 

CONSTRUCTIONS AND INITIALIZATION: 
-- constructor(): Pass.
-- setCallback: Sets the runEvent function.

INTERFACE:
-- queueEvent: Creates a new event in the stack on the event queue.
-- runEvent:   Runs event directly.
-- pollEvents: Runs the full queue stack until it is empty.

*/

#include "queue.hpp"
#include "event.hpp"

#ifndef EVENT_MANAGER_OLD_HPP
#define EVENT_MANAGER_OLD_HPP

#include <functional>

// === Constants ===

#define EVENT_QUEUE_LEN 16
#define EVENT2I_QUEUE_LEN 4
#define EVENT2F_QUEUE_LEN 4
#define EVENT4F_QUEUE_LEN 4

// === Classes ===


// ================ EventCall ================
typedef std::function<void(Event*)> EventCallTypeOld;
class EventCallOld {
private:
    EventCallTypeOld function;

public:
    EventCallOld();
    EventCallOld(EventCallTypeOld func_in);

    void operator()(Event* event);
};


// ================ EventManagerOld ================
class EventManagerOld {
private:
    EventCallOld propogateEventCall;
    
// --- Queues ---
private:
    Queue<Event, EVENT_QUEUE_LEN> event_queue;
    Queue<Event2i, EVENT2I_QUEUE_LEN> event2i_queue;
    Queue<Event2f, EVENT2F_QUEUE_LEN> event2f_queue;
    Queue<Event4f, EVENT4F_QUEUE_LEN> event4f_queue;

// --- Constructors and Initialization ---
public:
    EventManagerOld();
    void setCallback(EventCallOld event_call);

// --- Interface ---
public:
    void queueEvent(Module module_name, Data data);
    void queueEvent2i(Module module_name, Data data, vec2i vector);
    void queueEvent2f(Module module_name, Data data, vec2 vector);
    void queueEvent4f(Module module_name, Data data, vec4 vector);
    
    void propogateEvent(Event* event);
    void pollEvents();
};


// ================ EventInterfaceOld ================
typedef EnumMap<Data, EventCallOld, EVENT_DATA_LEN> FunctionArray;

class EventInterfaceOld {
private:
    FunctionArray function_array;
    EventManagerOld* event_manager;

public:
    EventInterfaceOld();
    void linkManager(EventManagerOld* ptr);
    void setCallback(Data data, EventCallTypeOld function);

    void operator()(Event* event);

    void queueEvent(Module module_name, Data data);
    void queueEvent2i(Module module_name, Data data, vec2i vector);
    void queueEvent2f(Module module_name, Data data, vec2 vector);
    void queueEvent4f(Module module_name, Data data, vec4 vector);

    void runEvent(Event* event);

    EventManagerOld* ptr();
};


// === Macros ===

#define PACK(FUNCTION_NAME) \
    std::bind(&FUNCTION_NAME, this, std::placeholders::_1)

#define CODE(CODENAME, DATA) \
    const EventCode CODENAME(EventType::ACTION, module_name, DATA);

#define CODE2I(CODENAME, DATA) \
    const EventCode CODENAME(EventType::VEC2I, module_name, DATA);

#endif