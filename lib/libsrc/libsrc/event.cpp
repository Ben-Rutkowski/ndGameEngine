#include <iostream>
#include "event.hpp"

// === Structs ===
EventCode::EventCode()
    :type{ EventType::null },
    module{ Module::null },
    operation{ Operation::null } {}

EventCode::EventCode(EventType type_in, Module module_in, Operation operation_in)
    :type{ type_in },
    module{ module_in },
    operation{ operation_in } {}

// === Initialization ===
ndEvent::ndEvent() {}

ndEvent::ndEvent(EventType type, Module module_name, Operation operation)
    :code{ EventCode(type, module_name, operation) } {}

ndEvent::ndEvent(Module module_name, Operation operation)
    :code{ EventCode(EventType::ACTION, module_name, operation) } {}

ndEvent::ndEvent(Module module_name, Operation op, vec4i int_vec)
    :code{ EventCode(EventType::VEC4I, module_name, op ) },
    vector_4i(int_vec) {}

// === Gets and Sets ===
EventType ndEvent::getType()   { return code.type; }
Module    ndEvent::getModule() { return code.module; }
Operation ndEvent::getOp()     { return code.operation; }
bool      ndEvent::operator!() { return code.type == EventType::null; }
// vec4      ndEvent::getVec4f()  { return vector_4f; }
vec4i     ndEvent::getVec4i()  { return vector_4i; }

// === Debugging ===
void ndEvent::print() {
    std::cout << moduleEnumToString(code.module);
    std::cout << " event: ";
    std::cout << operationEnumToString(code.operation) << std::endl;
}

void ndEvent::print(Module module_name) {
    std::cout << moduleEnumToString(code.module);
    std::cout << " event: ";
    std::cout << operationEnumToString(code.operation);
    std::cout << " in " << moduleEnumToString(module_name) << std::endl;
}
