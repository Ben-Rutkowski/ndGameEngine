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

/* CLASS: Event

Events are propogated throughout the application to
perform an action. 

Each Event has at least a Type, Module, Action.
- Different Types may have varing extra data (Event2i have an extra vec2i of attatched data).
- Module is the module in which the event is queued/created from.
- Action is the operation to trigger or toggle.

CONSTRUCTORS
- Event : Creates null Event with whose Type, Module and Action are all null.

- Event : Creats an event with the given code.
    ==Parameters==
        - EventCode code : The code for the event.


GETS AND SETS
- getType : Returns the type of the event.
    ==Return==
        - EventType

- getModule : Returns the module in which the event was created.
    ==Return==
        - Module

- getData : Returns the data of the event
    ==Return==
        - Data

VIRTUAL
- getInt : If given event has a int vector attatched, return
    the integer at the given index, else return 0;
    ==Parameter==
        - int index : the index of the vector.
    ==Return==
        - int

OPERATORS
- operator! : Returns true if event is null, else false.
        ==Return==
    - bool

DEBUGGING:
- print : Prints the event info

- print : Prints the event info and what module it is opened in.
    ==Parameter==
        - Module module_name : the name of the module that is opening it.

*/

class Event {
// --- Attributes ---
protected:
    EventCode code;

// --- Constructors ---
public:
    Event();
    // Event(EventCode code_in);
    Event(EventType type, Module module_name, Data data);
    Event(Module module_name, Data data);

// --- Gets and Sets ---
public:
    EventType getType();
    Module    getModule();
    Data      getData();

// --- Virtual ---
public:
    virtual int getInt(int index);

// --- Operators ---
public: 
    bool operator!();

// --- Debugging ---
public:
    void print();
    void print(Module module_name);
};

/* CLASS Event2i 
    Parent : Event

An Event2i extends Events with an attatched vec2i

*/

class Event2i : public Event {
private:
    vec2i vector_2i;

public:
    Event2i()
        :Event(),
        vector_2i() {}

    // Event2i(EventCode code, vec2i vec_input) 
    //     :Event(code),
    //     vector_2i(vec_input) {}

    // Event2i(EventCode code, int arg0, int arg1)
    //     :Event(code),
    //     vector_2i({arg0, arg1}) {}
        
    Event2i(Module module_name, Data data, int arg0, int arg1)
        :Event(EventType::VEC2I, module_name, data),
        vector_2i({arg0, arg1}) {}

    Event2i(Module module_name, Data data, vec2i vector)
        :Event(EventType::VEC2I, module_name, data),
        vector_2i{ vector } {}

    int getInt(int index) {
        return vector_2i[index];
    }
};

#endif
