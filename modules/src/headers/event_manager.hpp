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

#ifndef EVENT_MANAGER_HPP
#define EVENT_MANAGER_HPP

#include <functional>

// === Constants ===

#define EVENT_QUEUE_LEN 16
#define EVENT2I_QUEUE_LEN 4

// === Classes ===


// ================ EventCall ================
typedef std::function<void(Event*)> EventCallType;
class EventCall {
private:
    EventCallType function;

public:
    EventCall();
    EventCall(EventCallType func_in);

    void operator()(Event* event);
};


// ================ EventManager ================
class EventManager {
private:
    EventCall propogateEventCall;
    
// --- Queues ---
private:
    Queue<Event, EVENT_QUEUE_LEN> event_queue;
    Queue<Event2i, EVENT2I_QUEUE_LEN> event2i_queue;


// --- Constructors and Initialization ---
public:
    EventManager();
    void setCallback(EventCall event_call);

// --- Interface ---
public:
    // void queueEvent(EventCode code);
    // void queueEvent2i(EventCode code, int arg0, int arg1);
    // void queueEvent2i(EventCode code, vec2i vector);

    void queueEvent(Module module_name, Data data);
    void queueEvent2i(Module module_name, Data data, vec2i vector);
    
    void runEvent(Event* event);
    void pollEvents();
};


// ================ EventInterface ================
class EventInterface {
private:
    EventCall     function_array[EVENT_DATA_LEN];
    EventManager* event_manager;

public:
    EventInterface();
    void linkManager(EventManager* ptr);
    void setCallback(Data data, EventCallType function);

    void operator()(Event* event);
    
    // void queueEvent(EventCode code);
    // void queueEvent2i(EventCode code, int arg0, int arg1);
    // void queueEvent2i(EventCode code, vec2i vector);

    void queueEvent(Module module_name, Data data);
    void queueEvent2i(Module module_name, Data data, vec2i vector);

    void runEvent(Event* event);
};


// === Macros ===

#define PACK(FUNCTION_NAME) \
    std::bind(&FUNCTION_NAME, this, std::placeholders::_1)

#define CODE(CODENAME, DATA) \
    const EventCode CODENAME(EventType::ACTION, module_name, DATA);

#define CODE2I(CODENAME, DATA) \
    const EventCode CODENAME(EventType::VEC2I, module_name, DATA);

#endif