/*

Events are propogated throughout the application to
perform an action. 

Each Event has a Type, Module, Action and Time Queued/Created.
- Different Types may have varing extra data (e.g. Key events have a keycode).
- Module is the module in which the event is queued/created from.
- Action is the operation to trigger or toggle.
- Time is the clock time in which the event was created.

CONSTRUCTORS AND INITIALIZATION:
-- constructor(): Creates NULL Event.
-- constructor(EventType, Module, int): Creates a normal event with given arguments.

GETS AND SETS:
-- getType: Returns the EventType of the event.
-- getModule: Returns the Module of the event.
-- getData: Returns the data of the event.

DEBUGGING:
-- print: Prints event info.

*/

#include "common.hpp"
#include "vector.hpp"

#ifndef EVENT_H
#define EVENT_H

#include <iostream>

// === Structs ===
struct EventCode {
    EventType type;
    Module    module;
    Data      data;

    EventCode();
    EventCode(EventType type_in, Module module_in, Data data_in);
};

// === Classes ===
class Event {
protected:
    EventCode code;

public:
    // --- Constructors and Initialization
    Event();
    Event(EventCode code_in);

    // --- Gets and Sets ---
    EventType getType();
    Module    getModule();
    Data      getData();
    int       getIndex();

    virtual int getInt(int index);

    // --- Operators ---
    bool operator!();

    // --- Debugging ---
    void print();
};

class Event2i : public Event {
private:
    vec2i vector;

public:
    Event2i() {}
    Event2i(EventCode code, vec2i vec_input) 
        :Event(code), vector(vec_input) {}

    int getInt(int index) {
        return vector[index];
    }
};

#endif
