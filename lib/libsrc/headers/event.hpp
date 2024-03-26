#include "common.hpp"
#include "math/vector.hpp"

#ifndef EVENT_H
#define EVENT_H

// === Structs ===
struct EventCode {
    EventType type;
    Module    module;
    Operation operation;

    EventCode();
    EventCode(EventType type_in, Module module_in, Operation operation_in);
};

/* CLASS: Event

Events are propogated throughout the application to
perform an action. 

Each Event has at least a Type, Module, Action.
- Different Types may have varing extra data (Event2i have an extra vec2i of attatched data).
- Module is the module in which the event is queued/created from.
- Action is the operation to trigger or toggle.

*/

class ndEvent {
// --- Attributes ---
private:
    EventCode code;
    // vec4  vector_4f;
    vec4i vector_4i;

// --- Constructors ---
public:
    ndEvent();
    ndEvent(EventType type, Module module_name, Operation op);
    ndEvent(Module module_name, Operation op);
    ndEvent(Module module_name, Operation op, vec4i int_vec);

// --- Gets and Sets ---
public:
    EventType getType();
    Module    getModule();
    Operation getOp();
    // vec4      getVec4f();
    vec4i     getVec4i();
    bool      operator!();

// --- Debugging ---
public:
    void print();
    void print(Module module_name);
};

#endif
