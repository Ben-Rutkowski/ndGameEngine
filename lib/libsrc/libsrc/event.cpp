#include <iostream>
#include "event.hpp"

// === Structs ===
EventCode::EventCode()
    :type{ EventType::null },
    module{ Module::null },
    data{ Data::null } {}

EventCode::EventCode(EventType type_in, Module module_in, Data data_in)
    :type{ type_in },
    module{ module_in },
    data{ data_in } {}

// === Constructors and Initialization ===
Event::Event() {}

Event::Event(EventCode code_in)
    :code{ code_in } {}

// === Gets and Sets ===
EventType Event::getType()   { return code.type; }
Module    Event::getModule() { return code.module; }
Data      Event::getData()   { return code.data; }
int       Event::getIndex()  { return (int)code.data; }

int       Event::getInt(int place) { return 0; }

// === Operators ===
bool Event::operator!() { return code.type == EventType::null; }

// === Debugging ===
void Event::print() {
    std::cout << EnumMapIndex::moduleName(code.module);
    std::cout << " event: ";
    std::cout << EnumMapIndex::dataName(code.data) << std::endl;
}

void Event::print(Module module_name) {
    std::cout << EnumMapIndex::moduleName(code.module);
    std::cout << " event: ";
    std::cout << EnumMapIndex::dataName(code.data);
    std::cout << " in " << EnumMapIndex::moduleName(module_name) << std::endl;
}