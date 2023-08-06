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

#define EVENT_QUEUE_LEN 16

// === Classes ===

class EventManager
{
private:
    EventCall propogateEventCall;
    
// --- Queues ---
private:
    Queue<Event, EVENT_QUEUE_LEN> event_queue;

// --- Constructors and Initialization ---
public:
    EventManager();
    void setCallback(EventCall event_call);

    // --- Interface ---
    void queueEvent(EventCode code);
    void runEvent(Event* event);
    void pollEvents();
};


// ================ EventInterface ================
class EventInterface {
private:
    EventCall function_array[EVENT_DATA_LEN];
    EventManager* event_manager;

public:
    EventInterface();
    void linkManager(EventManager* ptr);
    void setCallback(Data data, EventCallType function);

    void operator()(Event* event);
    void queueEvent(EventCode code);
    void runEvent(Event* event);
};


// === Macros ===

#define PACK(FUNCTION_NAME) \
    std::bind(&FUNCTION_NAME, this, std::placeholders::_1)

#endif